#include "catalog.h"
#include "flexible.h"
#include "marshal.h"
#include "scan.h"

namespace shdb {

// Your code goes here.

Catalog::Catalog(std::shared_ptr<Store> store) : store_(store) {}

void Catalog::saveTableSchema(const std::filesystem::path &name, std::shared_ptr<Schema> schema)
{
    std::filesystem::path name_scheme = std::filesystem::path(name.string() + "scheme");
    if (!store_->checkTableExists(name_scheme)) {
        store_->createTable(name_scheme);
    }
    auto table = store_->openTable(name_scheme, createFlexiblePageProvider(fixed_schema));
    for (const auto &column : *schema) {
        if (column.type == Type::string) {
            auto row = Row{column.name, 0UL, static_cast<uint64_t>(column.length)/**/};
            table->insertRow(row);
        } else if (column.type == Type::boolean) {
            auto row = Row{column.name, 1UL, static_cast<uint64_t>(column.length)/**/};
            table->insertRow(row);
        } else if (column.type == Type::uint64) {
            auto row = Row{column.name, 2UL, static_cast<uint64_t>(column.length)/**/};
            table->insertRow(row);
        } else if (column.type == Type::int64) {
            auto row = Row{column.name, 3UL, static_cast<uint64_t>(column.length)/**/};
            table->insertRow(row);
        } else if (column.type == Type::varchar) {
            auto row = Row{column.name, 4UL, static_cast<uint64_t>(column.length)/**/};
            table->insertRow(row);
        }
    }
}

std::shared_ptr<Schema> Catalog::findTableSchema(const std::filesystem::path &name)
{

    std::filesystem::path name_scheme = std::filesystem::path(name.string() + "scheme");
    if (!store_->checkTableExists(name_scheme)) {

        return nullptr;
    }
    auto table = store_->openTable(name_scheme, createFlexiblePageProvider(fixed_schema));
    std::vector<ColumnSchema> types;
    for (auto row : shdb::Scan(table)) {
        if (row.size() == 0) {
            break;
        }
        int type = std::get<uint64_t>(row[1]);
        ColumnSchema cur_elem{std::get<std::string>(row[0]), Type::string, (int)std::get<uint64_t>(row[2])};
        if (type == 1) {
            cur_elem.type = Type::boolean;
        } else if (type == 2) {
            cur_elem.type = Type::uint64;
        } else if (type == 3) {
            cur_elem.type = Type::int64;
        } else if (type == 4) {
            cur_elem.type = Type::varchar;
        }
        types.push_back(cur_elem);
    }
    auto res = std::make_shared<Schema>(types);

    return res;
}

void Catalog::forgetTableSchema(const std::filesystem::path &name)
{
    std::filesystem::path name_scheme = std::filesystem::path(name.string() + "scheme");
    if (store_->checkTableExists(name_scheme)) {
        store_->removeTable(name_scheme);
    }
}



}    // namespace shdb
