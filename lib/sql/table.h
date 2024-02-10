#pragma once

#include <functional>
#include <memory>

#include "bufferpool.h"
#include "page_provider.h"
#include "row.h"

namespace shdb
{

class ITablePage : public IPage
{
public:
    virtual RowIndex getRowCount() = 0;

    virtual Row getRow(RowIndex index) = 0;

    virtual void deleteRow(RowIndex index) = 0;

    virtual std::pair<bool, RowIndex> insertRow(const Row & row) = 0;
};

class ITable
{
public:
    virtual ~ITable() = default;

    virtual PageIndex getPageCount() = 0;

    virtual std::shared_ptr<ITablePage> getPage(PageIndex page_index) = 0;

    virtual RowId insertRow(const Row & row) = 0;

    virtual Row getRow(RowId row_id) = 0;

    virtual void deleteRow(RowId row_id) = 0;
};

std::shared_ptr<ITable>
createTable(std::shared_ptr<BufferPool> buffer_pool, std::shared_ptr<File> file, std::shared_ptr<IPageProvider> provider);

}
