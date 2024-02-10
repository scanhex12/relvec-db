#include "database.h"

#include <iostream>
#include <memory>

#include "fixed.h"
#include "flexible.h"

namespace shdb
{

Database::Database(const std::filesystem::path & path, FrameIndex frame_count) : statistics(std::make_shared<Statistics>())
{
    store = std::make_shared<Store>(path, frame_count, statistics);
    catalog = std::make_shared<Catalog>(store);
    registerIndexes(*this);
}

std::shared_ptr<ITable> Database::createTable(const std::string & name, std::shared_ptr<Schema> schema)
{
    catalog->saveTableSchema(name, schema);
    auto provider = createPageProvider(std::move(schema));
    return store->createAndOpenTable(name, std::move(provider));
}

std::shared_ptr<ITable> Database::getTable(const std::string & name, std::shared_ptr<Schema> schema)
{
    if (!schema) {
        schema = catalog->findTableSchema(name);
    }
    auto provider = createPageProvider(schema);
    return store->openTable(name, std::move(provider));
}

bool Database::checkTableExists(const std::string & name)
{
    return store->checkTableExists(name);
}

void Database::dropTable(const std::string & name)
{
    catalog->forgetTableSchema(name);
    store->removeTable(name);
}

void Database::registerIndex(
    const std::string & index_type, IndexCreateCallback index_create_callback, IndexDropCallback index_drop_callback)
{
    index_type_to_callbacks.emplace(index_type, std::make_pair(std::move(index_create_callback), std::move(index_drop_callback)));
}

std::shared_ptr<IIndex>
Database::createIndex(const std::string & index_name, std::shared_ptr<Schema> key_schema, const std::string & index_type)
{
    if (key_schema->empty())
        throw std::runtime_error("Invalid empty index key schema");


    std::shared_ptr<Schema> index_schema = std::make_shared<Schema>(*key_schema);
    index_schema->push_back(ColumnSchema{index_type, Type::uint64, 0});
    catalog->saveTableSchema(index_name, index_schema);

    IndexMetadata metadata(index_name, std::move(key_schema));

    auto callbacks_it = index_type_to_callbacks.find(index_type);
    if (callbacks_it == index_type_to_callbacks.end())
        throw std::runtime_error("No index implementation registered for index type " + index_type);

    return callbacks_it->second.first(metadata, *store);
}

std::shared_ptr<IIndex> Database::getIndex(const std::string & index_name)
{
    auto index_schema = catalog->findTableSchema(index_name);
    if (!index_schema)
        throw std::runtime_error("No index exists with name " + index_name);

    std::string index_type = index_schema->back().name;
    index_schema->pop_back();

    IndexMetadata metadata(index_name, std::move(index_schema));
    auto callbacks_it = index_type_to_callbacks.find(index_type);
    if (callbacks_it == index_type_to_callbacks.end())
        throw std::runtime_error("No index implementation registered for index type " + index_type);

    return callbacks_it->second.first(metadata, *store);
}

void Database::dropIndex(const std::string & index_name)
{
    auto index_schema = catalog->findTableSchema(index_name);
    if (!index_schema)
        throw std::runtime_error("No index exists with name " + index_name);

    catalog->forgetTableSchema(index_name);

    std::string index_type = index_schema->back().name;
    index_schema->pop_back();

    IndexMetadata metadata(index_name, std::move(index_schema));
    auto callbacks_it = index_type_to_callbacks.find(index_type);
    if (callbacks_it == index_type_to_callbacks.end())
        throw std::runtime_error("No index implementation registered for index type " + index_type);

    callbacks_it->second.second(metadata, *store);
}

std::shared_ptr<Statistics> Database::getStatistics()
{
    return statistics;
}

std::shared_ptr<Schema> Database::findTableSchema(const std::filesystem::path & name)
{
    return catalog->findTableSchema(name);
}

std::shared_ptr<IPageProvider> Database::createPageProvider(std::shared_ptr<Schema> schema)
{
    for (const auto & column : *schema) {
        if (column.type == Type::string) {
            return createFlexiblePageProvider(std::move(schema));
        }
    }

    return createFixedPageProvider(std::move(schema));
}

std::shared_ptr<Database> connect(const std::string & path, FrameIndex frame_count)
{
    if (!std::filesystem::exists(path))
        std::filesystem::create_directories(path);

    return std::make_shared<Database>(path, frame_count);
}

}
