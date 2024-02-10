#include "file.h"

#include <filesystem>
#include <functional>

#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

namespace shdb
{

namespace
{

void safeSyscall(size_t count, const std::function<ssize_t()> & call)
{
    for (auto r = call(); r != static_cast<ssize_t>(count); r = call())
    {
        if (r == -1 && (errno != EINTR && errno != EAGAIN))
        {
            perror("Unable to perform I/O");
            throw std::runtime_error("Unable to perform I/O");
        }
    }
}

}

File::File(const std::filesystem::path & path, bool create)
{
    int flags = create ? (O_CREAT | O_EXCL) : 0;
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    fd = open(path.c_str(), O_DIRECT | O_SYNC | O_RDWR | flags, mode);
    if (fd == -1)
    {
        perror("Unable to open file");
        throw std::runtime_error("Unable to open file");
    }

    size = discoverSize();
}

File::~File()
{
    if (fd > 0)
        close(fd);
}

int File::getFd() const
{
    return fd;
}

off_t File::getSize() const
{
    return size;
}

off_t File::getPageCount() const
{
    return (getSize() + PageSize - 1) / PageSize;
}

void File::readPage(void * buf, PageIndex index) const
{
    read(buf, PageSize, PageSize * index);
}

void File::writePage(void * buf, PageIndex index) const
{
    write(buf, PageSize, PageSize * index);
}

PageIndex File::allocPage()
{
    PageIndex page_index = size / PageSize;
    alloc(PageSize);
    return page_index;
}

void File::sync()
{
    fdatasync(fd);
}

void File::read(void * buf, size_t count, off_t offset) const
{
    safeSyscall(count, [=, this] { return pread(fd, buf, count, offset); });
}

void File::write(void * buf, size_t count, off_t offset) const
{
    safeSyscall(count, [=, this] { return pwrite(fd, buf, count, offset); });
}

void File::alloc(off_t len)
{
    if (fallocate(fd, FALLOC_FL_ZERO_RANGE, size, len) == -1)
    {
        perror("Unable to allocate space to file");
        throw std::runtime_error("Unable to allocate space to file");
    }
    size += len;
}

size_t File::discoverSize() const
{
    struct stat statbuf;
    if (fstat(fd, &statbuf) == -1)
    {
        perror("Unable to get file stat");
        throw std::runtime_error("Unable to get file stat");
    }

    return statbuf.st_size;
}

}
