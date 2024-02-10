#pragma once

#include "table.h"
#include "row.h"

namespace shdb {

    class ScanIterator {
        // Your code goes here.
    public:
        ScanIterator(std::shared_ptr<ITable> table, PageIndex page_index = 0, RowIndex row_index = 0);


        RowId getRowId() const;

        Row getRow();

        Row operator*();

        bool operator==(const ScanIterator &other) const;

        bool operator!=(const ScanIterator &other) const;

        ScanIterator &operator++();

    private:

        std::shared_ptr<ITable> Table_;
        PageIndex PageIndex_;
        RowIndex RowIndex_;
    };

    class Scan {
        // Your code goes here.
    public:
        explicit Scan(std::shared_ptr<ITable> table);

        ScanIterator begin() const;

        ScanIterator end() const;

    private:
        std::shared_ptr<ITable> Table_;
    };

}    // namespace shdb
