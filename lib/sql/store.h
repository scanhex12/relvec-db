#pragma once

#include <filesystem>

#include "bufferpool.h"
#include "index_table.h"
#include "schema.h"
#include "statistics.h"
#include "table.h"

namespace shdb
{

class Store
{
public:
    Store(const std::filesystem::path & path, FrameIndex frame_count, std::shared_ptr<Statistics> statistics);

    void createTable(const std::filesystem::path & table_name);

    void createIndexTable(const std::filesystem::path & table_name);

    std::shared_ptr<ITable> openTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider);

    std::shared_ptr<IIndexTable> openIndexTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider);

    std::shared_ptr<ITable> createAndOpenTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider);

    std::shared_ptr<IIndexTable> createAndOpenIndexTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider);

    std::shared_ptr<ITable> createOrOpenTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider);

    std::shared_ptr<IIndexTable> createOrOpenIndexTable(const std::filesystem::path & table_name, std::shared_ptr<IPageProvider> provider);

    bool checkTableExists(const std::filesystem::path & table_name);

    bool removeTable(const std::filesystem::path & table_name);

    bool removeTableIfExists(const std::filesystem::path & table_name);

private:
    std::shared_ptr<BufferPool> buffer_pool;
    std::filesystem::path path;
};

}
