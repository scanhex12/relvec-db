#include "store.h"

#include <iostream>

namespace shdb
{

namespace
{

void throwTableAlreadyExistsError(const std::string & table_name)
{
    throw std::runtime_error("Table with name " + table_name + " already exists");
}

}

Store::Store(const std::filesystem::path & path, FrameIndex frame_count, std::shared_ptr<Statistics> statistics) : path(path)
{
    buffer_pool = std::make_shared<BufferPool>(std::move(statistics), frame_count);
}

void Store::createTable(const std::filesystem::path & table_name)
{
    if (checkTableExists(table_name))
        throwTableAlreadyExistsError(table_name);

    auto file = std::make_unique<File>(path / table_name, true /*create*/);
}

void Store::createIndexTable(const std::filesystem::path & table_name)
{
    if (checkTableExists(table_name))
        throwTableAlreadyExistsError(table_name);

    auto file = std::make_unique<File>(path / table_name, true);
}

std::shared_ptr<ITable> Store::openTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider)
{
    auto file = std::make_unique<File>(path / table_name, false /*create*/);
    return shdb::createTable(buffer_pool, std::move(file), std::move(provider));
}

std::shared_ptr<IIndexTable> Store::openIndexTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider)
{
    auto file = std::make_unique<File>(path / table_name, false /*create*/);
    return shdb::createIndexTable(buffer_pool, std::move(file), std::move(provider));
}

std::shared_ptr<ITable> Store::createAndOpenTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider)
{
    if (checkTableExists(table_name))
        throwTableAlreadyExistsError(table_name);

    auto file = std::make_unique<File>(path / table_name, true /*create*/);
    return shdb::createTable(buffer_pool, std::move(file), std::move(provider));
}

std::shared_ptr<IIndexTable>
Store::createAndOpenIndexTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider)
{
    if (checkTableExists(table_name))
        throwTableAlreadyExistsError(table_name);

    auto file = std::make_unique<File>(path / table_name, true /*create*/);
    return shdb::createIndexTable(buffer_pool, std::move(file), std::move(provider));
}

std::shared_ptr<ITable> Store::createOrOpenTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider)
{
    if (checkTableExists(table_name))
        return openTable(table_name, provider);

    return createAndOpenTable(table_name, provider);
}

std::shared_ptr<IIndexTable>
Store::createOrOpenIndexTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider)
{
    if (checkTableExists(table_name))
        return openIndexTable(table_name, provider);

    return createAndOpenIndexTable(table_name, provider);
}

bool Store::checkTableExists(const std::filesystem::path & table_name)
{
    return std::filesystem::exists(path / table_name);
}

bool Store::removeTable(const std::filesystem::path & table_name)
{
    return std::filesystem::remove(path / table_name);
}

bool Store::removeTableIfExists(const std::filesystem::path & table_name)
{
    if (checkTableExists(table_name))
        return removeTable(table_name);

    return false;
}
}
