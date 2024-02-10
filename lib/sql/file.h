#pragma once

#include <filesystem>

#include "page.h"

namespace shdb
{

class File
{
public:
    File(const std::filesystem::path & path, bool create);

    ~File();

    int getFd() const;

    off_t getSize() const;

    off_t getPageCount() const;

    void readPage(void * buf, PageIndex index) const;

    void writePage(void * buf, PageIndex index) const;

    PageIndex allocPage();

    void sync();

private:
    void read(void * buf, size_t count, off_t offset) const;

    void write(void * buf, size_t count, off_t offset) const;

    void alloc(off_t len);

    size_t discoverSize() const;

    int fd = -1;
    off_t size;
};

}
