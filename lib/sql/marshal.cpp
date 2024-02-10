#include "marshal.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <variant>

namespace shdb {

namespace {

template<class T>
void serialize_value(const T &value, uint8_t *&data)
{
    auto *vdata = reinterpret_cast<T *>(data);
    *vdata++ = value;
    data = reinterpret_cast<uint8_t *>(vdata);
}

template<class T>
T deserialize_value(uint8_t *&data)
{
    auto *vdata = reinterpret_cast<T *>(data);
    auto result = *vdata++;
    data = reinterpret_cast<uint8_t *>(vdata);
    return result;
}

}    // namespace

size_t Marshal::calculateFixedRowSpace(uint64_t nulls) const
{
    size_t result = sizeof(uint64_t);
    for (const auto &column : *schema) {
        auto null = nulls & 1;
        nulls >>= 1;
        if (null) {
            continue;
        }
        switch (column.type) {
        case Type::boolean: {
            result += sizeof(uint8_t);
            break;
        }
        case Type::uint64: {
            result += sizeof(uint64_t);
            break;
        }
        case Type::int64: {
            result += sizeof(int64_t);
            break;
        }

        case Type::varchar: {
            result += column.length;
            break;
        }
        case Type::string: {
            //throw std::runtime_error("can't compute string as fixed size type");
            result += 1;
            break;
        }
        default: {
            std::cout << (int)column.type << '\n';
            assert(0);
        }
        }
    }
    return result;
}

uint64_t Marshal::getNulls(const Row &row) const
{
    uint64_t nulls = 0;
    for (size_t index = 0; index < row.size(); ++index) {
        if (std::holds_alternative<Null>(row[index])) {
            nulls |= (1UL << index);
        }
    }
    return nulls;
}

Marshal::Marshal(std::shared_ptr<Schema> schema)
    : schema(std::move(schema)), fixed_row_space(calculateFixedRowSpace(0))
{
    assert(Marshal::schema->size() <= sizeof(uint64_t) * 8);
}

size_t Marshal::getFixedRowSpace() const
{
    return fixed_row_space;
}

size_t Marshal::getRowSpace(const Row &row) const
{
    auto nulls = getNulls(row);
    //std::cout << "Here\n";

    size_t result = calculateFixedRowSpace(nulls);
    for (size_t index = 0; index < schema->size(); ++index) {
        if (nulls & (1UL << index)) {
            continue;
        }
        switch ((*schema)[index].type) {
        case Type::string: {
            // Your code goes here.
            result += std::get<std::string>(row[index]).size();
            break;
        }
        default:
            break;
        }
    }
    return result;
}

void Marshal::serializeRow(uint8_t *data, const Row &row) const
{
    assert(row.size() < 64);
    uint64_t nulls = getNulls(row);
    //auto start = data;
    serialize_value<uint64_t>(nulls, data);
    for (size_t index = 0; index < schema->size(); ++index) {
        if (nulls & (1UL << index)) {
            continue;
        }

        switch ((*schema)[index].type) {
        case Type::boolean: {
            auto value = static_cast<uint8_t>(std::get<bool>(row[index]));
            serialize_value(value, data);
            break;
        }
        case Type::uint64: {
            auto value = std::get<uint64_t>(row[index]);
            serialize_value(value, data);
            break;
        }
        case Type::int64: {
            auto value = std::get<int64_t>(row[index]);
            serialize_value(value, data);
            break;
        }

        case Type::varchar: {
            auto &str = std::get<std::string>(row[index]);
            auto length = (*schema)[index].length;
            ::memcpy(data, str.c_str(), str.size());
            ::memset(data + str.size(), 0, length - str.size());
            data += length;
            break;
        }
        case Type::string: {
            // Your code goes here.

            
            auto &str = std::get<std::string>(row[index]);

            serialize_value(static_cast<uint8_t>(str.size()), data);
            for (size_t i = 0; i < str.size(); ++i) {
                serialize_value(static_cast<uint8_t>(str[i]), data);
            }
            break;
        }
        }

    }
    //assert(static_cast<size_t>(data - start) == calculate_fixed_row_space(nulls));
}

Row Marshal::deserializeRow(uint8_t *data) const
{
    //auto *start = data;
    auto nulls = deserialize_value<uint64_t>(data);
    int cnt = 0;
    Row row;
    for (size_t index = 0; index < schema->size(); ++index) {
        if (nulls & (1UL << index)) {
            row.emplace_back(Null{});
            continue;
        }
        switch ((*schema)[index].type) {
        case Type::boolean: {
            auto value = deserialize_value<uint8_t>(data);
            row.emplace_back(static_cast<bool>(value));
            cnt++;
            break;
        }
        case Type::uint64: {
            auto value = deserialize_value<uint64_t>(data);
            row.emplace_back(value);
            cnt++;
            break;
        }
        case Type::int64: {
            auto value = deserialize_value<int64_t>(data);
            row.emplace_back(value);
            cnt++;
            break;
        }
        case Type::varchar: {
            auto length = strnlen(reinterpret_cast<char *>(data), (*schema)[index].length);
            auto str = std::string(reinterpret_cast<char *>(data), length);
            row.emplace_back(std::move(str));
            data += (*schema)[index].length;
            break;
        }
        case Type::string: {
            // Your code goes here.
            
            auto length = deserialize_value<uint8_t>(data);
            std::string str(length, ' ');
            for (uint64_t i = 0; i < length; ++i) {
                str[i] = deserialize_value<uint8_t>(data);
            }
            row.emplace_back(str);

            break;
        }
        }
    }
    //assert(static_cast<size_t>(data - start) == calculate_fixed_row_space(nulls));
    return row;
}

}    // namespace shdb
