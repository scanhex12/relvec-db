#include "row.h"

#include <sstream>

namespace shdb
{

std::string toString(const Value & value)
{
    return std::visit(
        Overloaded{
            [&](const Null &) -> std::string { return "NULL"; },
            [&](const uint64_t & value) -> std::string { return std::to_string(value); },
            [&](const int64_t & value) -> std::string { return std::to_string(value); },
            [&](const bool & value) -> std::string { return (value ? "true" : "false"); },
            [&](const std::string & value) -> std::string { return value; },
        },
        value);
}

std::string toString(const Row & row)
{
    std::stringstream stream;
    stream << '[';
    for (size_t index = 0; index < row.size(); ++index)
    {
        const auto & value = row[index];
        stream << toString(value);
        if (index < row.size() - 1)
            stream << ',';
    }
    stream << ']';
    return stream.str();
}

std::string toString(const RowId & row_id)
{
    std::stringstream stream;
    stream << '(';
    stream << row_id.page_index;
    stream << ',';
    stream << row_id.row_index;
    stream << ')';

    return stream.str();
}

std::ostream & operator<<(std::ostream & stream, const Value & value)
{
    stream << toString(value);
    return stream;
}

std::ostream & operator<<(std::ostream & stream, const Row & row)
{
    stream << toString(row);
    return stream;
}

std::ostream & operator<<(std::ostream & stream, const RowId & row_id)
{
    stream << toString(row_id);
    return stream;
}

bool Null::operator==(const Null &) const
{
    return true;
}

bool Null::operator!=(const Null &) const
{
    return false;
}

bool RowId::operator==(const RowId & other) const
{
    return page_index == other.page_index && row_index == other.row_index;
}

bool RowId::operator!=(const RowId & other) const
{
    return !(*this == other);
}

}
