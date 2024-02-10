#include "index.h"

#include <memory>
#include <optional>
#include <vector>

#include "btree.h"
#include "database.h"

namespace shdb
{

bool IndexMetadata::hasVariableKeys() const
{
    for (const auto & key_column : *key_schema)
        if (key_column.type == Type::string)
            return false;

    return true;
}

size_t IndexMetadata::fixedKeySizeInBytes() const
{
    return key_schema_marshal->getFixedRowSpace();
}

KeyCondition createEQCondition(const ColumnSchema & column, Value value)
{
    return KeyCondition{column, IndexComparator::equal, std::move(value)};
}

KeyCondition createNECondition(const ColumnSchema & column, Value value)
{
    return KeyCondition{column, IndexComparator::notEqual, std::move(value)};
}

KeyCondition createGTCondition(const ColumnSchema & column, Value value)
{
    return KeyCondition{column, IndexComparator::greater, std::move(value)};
}

KeyCondition createGECondition(const ColumnSchema & column, Value value)
{
    return KeyCondition{column, IndexComparator::greaterOrEqual, std::move(value)};
}

KeyCondition createLTCondition(const ColumnSchema & column, Value value)
{
    return KeyCondition{column, IndexComparator::less, std::move(value)};
}

KeyCondition createLECondition(const ColumnSchema & column, Value value)
{
    return KeyCondition{column, IndexComparator::lessOrEqual, std::move(value)};
}

IIndex::IIndex(IndexMetadata metadata_) : metadata(metadata_)
{
    if (!supportVariableLengthKeys() && !metadata.hasVariableKeys())
        throw std::runtime_error("Index does not support variable length keys");
}

void registerIndexes(Database & database)
{
    database.registerIndex(
        "btree",
        [](const IndexMetadata & metadata, Store & store) { return BTree::createIndex(metadata, store); },
        [](const IndexMetadata & metadata, Store & store) { BTree::removeIndex(metadata.getIndexName(), store); });
}

}
