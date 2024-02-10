#pragma once

#include "row.h"
#include "schema.h"

namespace shdb
{

class RowSet
{
public:
    explicit RowSet() = default;

    explicit RowSet(std::shared_ptr<Schema> schema_, Rows rows_ = {}) : schema(std::move(schema_)), rows(std::move(rows_)) { }

    const std::shared_ptr<Schema> & getSchema() const { return schema; }

    const Rows & getRows() const { return rows; }

    void addRow(Row row) { rows.push_back(row); }

private:
    std::shared_ptr<Schema> schema;
    Rows rows;
};

}
