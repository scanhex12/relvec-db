#pragma once

#include <memory>

#include "cache.h"
#include "file.h"
#include "page.h"
#include "statistics.h"

namespace shdb
{

using FrameIndex = int;

class FramePool
{
public:
    FramePool(std::shared_ptr<Statistics> statistics, FrameIndex frame_count);

    ~FramePool();

    FramePool(const FramePool & other) = delete;

    std::pair<FrameIndex, uint8_t *> acquireFrame(std::shared_ptr<File> file, PageIndex page_index);

    void releaseFrame(FrameIndex frame_index);

private:
    struct Frame
    {
        uint8_t * data = nullptr;
        std::shared_ptr<File> file;
        PageIndex page_index = 0;
        size_t ref_count = 0;
    };

    void dumpFrame(Frame & frame);

    uint8_t * data;
    std::vector<Frame> frames;
    ClockCache<PageId, FrameIndex> cache;
    std::shared_ptr<Statistics> statistics;
};

class Frame
{
public:
    Frame(std::shared_ptr<FramePool> frame_pool, FrameIndex frame_index, uint8_t * data);

    ~Frame();

    const uint8_t * getData() const { return data; }

    uint8_t * getData() { return data; }

private:
    std::shared_ptr<FramePool> frame_pool;
    size_t frame_index;
    uint8_t * data;
};

class BufferPool
{
public:
    BufferPool(std::shared_ptr<Statistics> statistics, FrameIndex frame_count);

    std::shared_ptr<Frame> getPage(std::shared_ptr<File> file, PageIndex page_index);

private:
    friend class Frame;
    std::shared_ptr<FramePool> frame_pool;
};

}
