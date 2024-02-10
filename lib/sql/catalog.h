#pragma once

#include "schema.h"
#include "store.h"
#include "table.h"

#include <map>

namespace shdb {


class Catalog
{
    // Your code goes here.
    std::shared_ptr<Store> store_;
    std::shared_ptr<Schema> fixed_schema = std::make_shared<Schema>(Schema{{"A", Type::string},
                                                                           {"B", Type::uint64},
                                                                           {"id", Type::uint64},
                                                                            });

public:
    explicit Catalog(std::shared_ptr<Store> store);

    void saveTableSchema(const std::filesystem::path &name, std::shared_ptr<Schema> schema);
    std::shared_ptr<Schema> findTableSchema(const std::filesystem::path &name);
    void forgetTableSchema(const std::filesystem::path &name);
};

}    // namespace shdb
