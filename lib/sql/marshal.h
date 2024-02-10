#pragma once

#include "row.h"
#include "schema.h"

#include <memory>

namespace shdb {

class Marshal
{
public:
    std::shared_ptr<Schema> schema;
    size_t fixed_row_space;

    size_t calculateFixedRowSpace(uint64_t nulls) const;
    uint64_t getNulls(const Row &row) const;

    explicit Marshal(std::shared_ptr<Schema> schema);

    size_t getFixedRowSpace() const;
    size_t getRowSpace(const Row &row) const;
    void serializeRow(uint8_t *data, const Row &row) const;
    Row deserializeRow(uint8_t *data) const;
};

}    // namespace shdb
