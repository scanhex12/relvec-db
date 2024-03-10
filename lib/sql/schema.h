#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace shdb
{

enum class Type
{
    boolean,
    uint64,
    int64,
    varchar,
    string,
    vector
};

using Types = std::vector<Type>;

std::string toString(Type type);

std::ostream & operator<<(std::ostream & stream, Type type);

struct ColumnSchema
{
    ColumnSchema(std::string name_, Type type_, uint32_t length_ = 0) : name(std::move(name_)), type(type_), length(length_) { }

    ColumnSchema(Type type_, uint32_t length_ = 0) : type(type_), length(length_) { } /// NOLINT

    ColumnSchema() = default;

    std::string name;
    Type type;
    uint32_t length;

    size_t operator()(const ColumnSchema& cs) const {
        return std::hash<std::string>()(cs.name) ^
               std::hash<int>()(static_cast<int>(cs.type)) ^
               std::hash<uint32_t>()(cs.length);
    }

    bool operator==(const ColumnSchema & other) const
    {
        return std::tie(name, type, length) == std::tie(other.name, other.type, other.length);
    }
};

std::string toString(const ColumnSchema & schema);

std::ostream & operator<<(std::ostream & stream, const ColumnSchema & schema);

using Schema = std::vector<ColumnSchema>;
using SchemePtr = std::shared_ptr<Schema>;

static SchemePtr createSchema(const std::vector<ColumnSchema> & column_schemas)
{
    return std::make_shared<Schema>(column_schemas);
}

static SchemePtr createSchema(std::vector<ColumnSchema> && column_schemas)
{
    return std::make_shared<Schema>(std::move(column_schemas));
}

std::string toString(const Schema & schema);

std::ostream & operator<<(std::ostream & stream, const Schema & schema);

Schema deserializeSchema(const std::string& serializedSchema);

}
