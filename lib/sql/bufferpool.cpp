#include "bufferpool.h"

#include "malloc.h"

#include <cassert>

namespace shdb
{

FramePool::FramePool(std::shared_ptr<Statistics> statistics_, FrameIndex frame_count)
    : frames(frame_count)
    , cache(
          [=]
          {
              std::vector<FrameIndex> free_frames(frame_count);
              FrameIndex index = 0;
              std::generate_n(free_frames.begin(), frame_count, [&] { return index++; });
              return free_frames;
          }())
    , statistics(std::move(statistics_))
{
    data = reinterpret_cast<uint8_t *>(pvalloc(frame_count * PageSize));
    for (FrameIndex index = 0; index < frame_count; ++index)
        frames[index].data = data + index * PageSize;
}

FramePool::~FramePool()
{
    for (auto & frame : frames)
        dumpFrame(frame);
    free(data);
}

void FramePool::dumpFrame(Frame & frame)
{
    if (frame.file)
    {
        frame.file->writePage(frame.data, frame.page_index);
        ++statistics->page_written;
    }
}

std::pair<FrameIndex, uint8_t *> FramePool::acquireFrame(std::shared_ptr<File> file, PageIndex page_index)
{
    FrameIndex frame_index;
    auto key = std::make_pair(file->getFd(), page_index);
    if (auto [found, index] = cache.find(key); found)
    {
        frame_index = index;
        assert(frames[frame_index].page_index == page_index);
        assert(frames[frame_index].file->getFd() == file->getFd());
    }
    else
    {
        frame_index = cache.put(key);
        auto & frame = frames[frame_index];
        dumpFrame(frame);
        frame.file = std::move(file);
        frame.page_index = page_index;
        frame.file->readPage(frame.data, frame.page_index);
        ++statistics->page_read;
    }
    ++frames[frame_index].ref_count;
    if (frames[frame_index].ref_count == 1)
        cache.lock(key);
    ++statistics->page_accessed;
    return {frame_index, frames[frame_index].data};
}

void FramePool::releaseFrame(FrameIndex frame_index)
{
    auto & frame = frames[frame_index];
    --frame.ref_count;
    if (frame.ref_count == 0)
        cache.unlock(std::make_pair(frame.file->getFd(), frame.page_index));
}

Frame::Frame(std::shared_ptr<FramePool> frame_pool, FrameIndex frame_index, uint8_t * data)
    : frame_pool(std::move(frame_pool)), frame_index(frame_index), data(data)
{
}

Frame::~Frame()
{
    if (frame_pool)
        frame_pool->releaseFrame(frame_index);
}

BufferPool::BufferPool(std::shared_ptr<Statistics> statistics, FrameIndex frame_count)
    : frame_pool(std::make_shared<FramePool>(std::move(statistics), frame_count))
{
}

std::shared_ptr<Frame> BufferPool::getPage(std::shared_ptr<File> file, PageIndex page_index)
{
    auto [frame_index, data] = frame_pool->acquireFrame(file, page_index);
    return std::make_shared<Frame>(frame_pool, frame_index, data);
}

}
