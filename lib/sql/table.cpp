#include "table.h"

namespace shdb
{

class ImplementedTable : public ITable
{
public:
    ImplementedTable(std::shared_ptr<BufferPool> buffer_pool, std::shared_ptr<File> file, std::shared_ptr<IPageProvider> page_provider)
        : buffer_pool(std::move(buffer_pool)), file(std::move(file)), page_provider(std::move(page_provider))
    {
    }

    RowId insertRow(const Row & row) override
    {
        while (true)
        {
            if (current_page_index == getPageCount())
                current_page_index = file->allocPage();

            auto page = getPage(current_page_index);
            auto [inserted, row_index] = page->insertRow(row);
            if (inserted)
                return RowId{current_page_index, row_index};

            ++current_page_index;
        }
    }

    Row getRow(RowId row_id) override
    {
        auto page = getPage(row_id.page_index);
        auto row = page->getRow(row_id.row_index);
        return row;
    }

    void deleteRow(RowId row_id) override
    {
        auto page = getPage(row_id.page_index);
        page->deleteRow(row_id.row_index);
    }

    PageIndex getPageCount() override { return file->getPageCount(); }

    std::shared_ptr<ITablePage> getPage(PageIndex page_index) override
    {
        auto frame = buffer_pool->getPage(file, page_index);
        return std::static_pointer_cast<ITablePage>(page_provider->getPage(std::move(frame)));
    }

private:
    std::shared_ptr<BufferPool> buffer_pool;
    std::shared_ptr<File> file;
    std::shared_ptr<IPageProvider> page_provider;
    PageIndex current_page_index = 0;
};

std::shared_ptr<ITable>
createTable(std::shared_ptr<BufferPool> buffer_pool, std::shared_ptr<File> file, std::shared_ptr<IPageProvider> page_provider)
{
    return std::make_shared<ImplementedTable>(std::move(buffer_pool), std::move(file), std::move(page_provider));
}

}
