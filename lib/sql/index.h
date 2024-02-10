#pragma once

#include "marshal.h"
#include "row.h"
#include "scan.h"
#include "schema.h"

#include <memory>
#include <optional>
#include <vector>

namespace shdb
{

using IndexKey = Row;

class IndexMetadata
{
public:
    IndexMetadata(const std::string & index_name_, std::shared_ptr<Schema> key_schema_)
        : index_name(index_name_), key_schema(std::move(key_schema_)), key_schema_marshal(std::make_shared<Marshal>(key_schema))
    {
        std::cout << "OK INSIDE METADATA\n";
    }

    const std::string & getIndexName() const { return index_name; }

    const std::shared_ptr<Schema> & getKeySchema() const { return key_schema; }

    const std::shared_ptr<Marshal> & getKeyMarshal() const { return key_schema_marshal; }

    bool hasVariableKeys() const;

    size_t fixedKeySizeInBytes() const;

private:
    std::string index_name;
    std::shared_ptr<Schema> key_schema;
    std::shared_ptr<Marshal> key_schema_marshal;
};

enum class IndexComparator
{
    equal,
    notEqual,
    greater,
    greaterOrEqual,
    less,
    lessOrEqual,
};

/** KeyCondition represents a single condition for selecting rows
  * based on the comparison of column with constant value.
  */
struct KeyCondition
{
    ColumnSchema column;
    IndexComparator comparator;
    Value value;
};

/// Multiple KeyConditions that are combined using a logical conjunction (AND operator)
using KeyConditions = std::vector<KeyCondition>;

KeyCondition createEQCondition(const ColumnSchema & column, Value value);

KeyCondition createNECondition(const ColumnSchema & column, Value value);

KeyCondition createGTCondition(const ColumnSchema & column, Value value);

KeyCondition createGECondition(const ColumnSchema & column, Value value);

KeyCondition createLTCondition(const ColumnSchema & column, Value value);

KeyCondition createLECondition(const ColumnSchema & column, Value value);

class IIndexIterator
{
public:
    virtual ~IIndexIterator() = default;

    virtual std::optional<std::pair<IndexKey, RowId>> nextRow() = 0;
};

class IIndex
{
public:
    explicit IIndex(IndexMetadata metadata_);

    virtual ~IIndex() = default;

    const IndexMetadata & getMetadata() const { return metadata; }

    /// Returns true if index supports variable length keys
    virtual bool supportVariableLengthKeys() { return false; }

    /// Returns true if index cannot contain dublicate keys.
    virtual bool isUnique() { return true; }

    /// Insert index key with row id into the index
    virtual void insert(const IndexKey & index_key, const RowId & row_id) = 0;

    /// Remove index key with row id from the index, returns true if key existed in index, false otherwise
    virtual bool remove(const IndexKey & index_key, const RowId & row_id) = 0;

    /// Lookup index key
    virtual void lookup(const IndexKey & index_key, std::vector<RowId> & result) = 0;

    /// Lookup unique index key
    virtual std::optional<RowId> lookupUniqueKey(const IndexKey & index_key)
    {
        std::vector<RowId> result;
        lookup(index_key, result);
        if (result.empty()) {
            return {};
        }
        return result[0];
    }

    /// Returns an iterator for full index scan
    virtual std::unique_ptr<IIndexIterator> read() = 0;

    /// Returns an iterator for selective index scan using the provided predicates
    virtual std::unique_ptr<IIndexIterator> read(const KeyConditions & predicates) = 0;

    const IndexMetadata metadata;
};

using IndexPtr = std::shared_ptr<IIndex>;

class Database;
void registerIndexes(Database & database);

}
