#pragma once

#include "page.h"

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace shdb
{

template <class... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

struct Null
{
    bool operator==(const Null & other) const;

    bool operator!=(const Null & other) const;
};

using Value = std::variant<Null, bool, uint64_t, int64_t, std::string>;
using Row = std::vector<Value>;
using Rows = std::vector<Row>;
using RowIndex = int32_t;

struct RowId
{
    PageIndex page_index;
    RowIndex row_index;

    bool operator==(const RowId & other) const;

    bool operator!=(const RowId & other) const;
};

std::string toString(const Value & value);

std::string toString(const Row & row);

std::string toString(const RowId & row);

std::ostream & operator<<(std::ostream & stream, const Value & value);

std::ostream & operator<<(std::ostream & stream, const Row & row);

std::ostream & operator<<(std::ostream & stream, const RowId & row);
}

template <typename T>
inline void hashCombine(size_t & seed, const T & v)
{
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{

template <>
class hash<shdb::RowId>
{
public:
    size_t operator()(const shdb::RowId & row_id) const { return std::hash<int>()(row_id.page_index) ^ std::hash<int>()(row_id.row_index); }
};

template <>
class hash<shdb::Null>
{
public:
    size_t operator()(const shdb::Null &) const { return 0x12345678; }
};

template <>
class hash<shdb::Row>
{
public:
    size_t operator()(const shdb::Row & row) const
    {
        size_t seed = 0;
        for (const auto & value : row)
            hashCombine(seed, value);
        return seed;
    }
};

}
