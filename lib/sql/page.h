#pragma once

#include <cstdint>
#include <functional>
#include <memory>

namespace shdb
{

constexpr uint32_t PageSize = 4096;

using PageIndex = int32_t;
constexpr int32_t InvalidPageIndex = -1;

using PageId = std::pair<int32_t, PageIndex>;

class IPage
{
public:
    virtual ~IPage() = default;
};
}

namespace std
{

template <>
class hash<shdb::PageId>
{
public:
    size_t operator()(const shdb::PageId & page_id) const
    {
        return std::hash<int32_t>()(page_id.first) ^ std::hash<int32_t>()(page_id.second);
    }
};

}
