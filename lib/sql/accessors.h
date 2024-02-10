#pragma once

#include <optional>

#include "row.h"
#include "schema.h"

namespace shdb
{

class SchemaAccessor
{
public:
    explicit SchemaAccessor();

    explicit SchemaAccessor(std::shared_ptr<Schema> schema_);

    bool hasColumn(const std::string & name);

    std::optional<size_t> getColumnIndexOrNull(const std::string & name);

    size_t getColumnIndexOrThrow(const std::string & name);

    std::optional<ColumnSchema> getColumnOrNull(const std::string & name);

    const ColumnSchema & getColumnOrThrow(const std::string & name);

    void addColumn(ColumnSchema column);

private:
    std::shared_ptr<Schema> schema;

    std::unordered_map<std::string, size_t> column_name_to_index;
};

using SchemaAccessorPtr = std::shared_ptr<SchemaAccessor>;

}
