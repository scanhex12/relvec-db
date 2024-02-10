#pragma once

#include <filesystem>

#include "catalog.h"
#include "index.h"
#include "schema.h"
#include "statistics.h"
#include "store.h"
#include "table.h"

namespace shdb
{

class Database
{
public:
    Database(const std::filesystem::path & path, FrameIndex frame_count);

    std::shared_ptr<ITable> createTable(const std::string & table_name, std::shared_ptr<Schema> schema);

    std::shared_ptr<ITable> getTable(const std::string & name, std::shared_ptr<Schema> schema = nullptr);

    bool checkTableExists(const std::string & name);

    void dropTable(const std::string & name);

    using IndexCreateCallback = std::function<std::shared_ptr<IIndex>(const IndexMetadata &, Store &)>;

    using IndexDropCallback = std::function<void(const IndexMetadata &, Store & store)>;

    void registerIndex(const std::string & index_type, IndexCreateCallback index_create_callback, IndexDropCallback index_drop_callback);

    std::shared_ptr<IIndex> createIndex(const std::string & index_name, std::shared_ptr<Schema> key_schema, const std::string & index_type);

    std::shared_ptr<IIndex> getIndex(const std::string & index_name);

    void dropIndex(const std::string & index_name);

    std::shared_ptr<Statistics> getStatistics();

    std::shared_ptr<Schema> findTableSchema(const std::filesystem::path & name);

    std::shared_ptr<Store> getStore() const { return store; }

private:
    std::shared_ptr<IPageProvider> createPageProvider(std::shared_ptr<Schema> schema);

    std::shared_ptr<Statistics> statistics;
    std::shared_ptr<Store> store;
    std::shared_ptr<Catalog> catalog;
    std::unordered_map<std::string, std::pair<IndexCreateCallback, IndexDropCallback>> index_type_to_callbacks;
};

std::shared_ptr<Database> connect(const std::string & path, FrameIndex frame_count);

class Foo {};
}
