#include "index_table.h"

namespace shdb
{

class IndexTable : public IIndexTable
{
public:
    IndexTable(std::shared_ptr<BufferPool> buffer_pool, std::shared_ptr<File> file, std::shared_ptr<IPageProvider> page_provider)
        : buffer_pool(std::move(buffer_pool)), file(std::move(file)), page_provider(std::move(page_provider))
    {
    }

    PageIndex getPageCount() override { return file->getPageCount(); }

    std::shared_ptr<IPage> getPage(PageIndex page_index) override
    {
        auto frame = buffer_pool->getPage(file, page_index);
        return page_provider->getPage(std::move(frame));
    }

    PageIndex allocatePage() override
    {
        current_page_index = file->allocPage();
        return current_page_index;
    }

private:
    std::shared_ptr<BufferPool> buffer_pool;
    std::shared_ptr<File> file;
    std::shared_ptr<IPageProvider> page_provider;
    PageIndex current_page_index = 0;
};

std::shared_ptr<IIndexTable>
createIndexTable(std::shared_ptr<BufferPool> buffer_pool, std::shared_ptr<File> file, std::shared_ptr<IPageProvider> page_provider)
{
    return std::make_shared<IndexTable>(std::move(buffer_pool), std::move(file), std::move(page_provider));
}

}
