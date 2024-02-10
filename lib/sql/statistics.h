#pragma once

#include <cstdint>
#include <string>

namespace shdb
{

struct Statistics
{
    uint64_t page_read = 0;
    uint64_t page_written = 0;
    uint64_t page_accessed = 0;
};

std::string toString(const Statistics & statistics);

}
