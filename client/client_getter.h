#pragma once

#include "client_config.h"
#include <memory>
#include <vector>
#include <map>
#include "../lib/sql/row.h"
#include "../lib/sql/schema.h"

class ClientGetter {
public:
    using HostInfo = std::pair<std::string, int>; // {host, port}

    ClientGetter(std::shared_ptr<ClientConfig> config);

    std::vector<std::vector<HostInfo>> GetChunks(const std::vector<std::string>& tables); // table to host

    std::shared_ptr<shdb::Schema> GetTableSchema(const std::string& table);

    void AddTableOnMaster(std::string table, size_t tableSize);

    void RemoveTableOnMaster(std::string table);

    std::vector<shdb::Row> GetTableContent(std::string table);

    void InsertRowOnTable(std::string table, shdb::Row row);

    void CreateTable(std::string table, std::shared_ptr<shdb::Schema> schema);

private:
    std::shared_ptr<ClientConfig> config_;
};
