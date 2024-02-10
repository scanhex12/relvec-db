#pragma once

#include <functional>
#include <memory>

#include "bufferpool.h"
#include "page_provider.h"
#include "row.h"

namespace shdb
{

class IIndexTable
{
public:
    virtual ~IIndexTable() = default;

    virtual PageIndex getPageCount() = 0;

    virtual std::shared_ptr<IPage> getPage(PageIndex page_index) = 0;

    virtual PageIndex allocatePage() = 0;
};

std::shared_ptr<IIndexTable>
createIndexTable(std::shared_ptr<BufferPool> buffer_pool, std::shared_ptr<File> file, std::shared_ptr<IPageProvider> provider);

}
