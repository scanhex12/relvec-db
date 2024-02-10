#include "accessors.h"

#include "row.h"

#include <cassert>
#include <stdexcept>

namespace shdb
{

SchemaAccessor::SchemaAccessor() : schema(std::make_shared<Schema>())
{
}

SchemaAccessor::SchemaAccessor(std::shared_ptr<Schema> schema_) : schema(std::move(schema_))
{
    if (schema)
    {
        size_t index = 0;
        for (const auto & column : *schema)
            column_name_to_index[column.name] = index++;
    }
}

bool SchemaAccessor::hasColumn(const std::string & name)
{
    return column_name_to_index.find(name) != column_name_to_index.end();
}

std::optional<size_t> SchemaAccessor::getColumnIndexOrNull(const std::string & name)
{
    auto it = column_name_to_index.find(name);
    if (it == column_name_to_index.end())
        return {};

    return it->second;
}

size_t SchemaAccessor::getColumnIndexOrThrow(const std::string & name)
{
    auto it = column_name_to_index.find(name);
    if (it == column_name_to_index.end())
        throw "Schema does not contain column " + name;

    return it->second;
}

std::optional<ColumnSchema> SchemaAccessor::getColumnOrNull(const std::string & name)
{
    auto column_index = getColumnIndexOrNull(name);
    if (!column_index)
        return {};

    return (*schema)[*column_index];
}

const ColumnSchema & SchemaAccessor::getColumnOrThrow(const std::string & name)
{
    auto column_index = getColumnIndexOrThrow(name);
    return (*schema)[column_index];
}

void SchemaAccessor::addColumn(ColumnSchema column)
{
    if (hasColumn(column.name))
        throw "Schema already contains column " + column.name;

    column_name_to_index[column.name] = schema->size();
    schema->push_back(std::move(column));
}

}
