#include "fixed.h"

#include <cassert>
#include <cstring>

#include "marshal.h"
#include "row.h"
#include "table.h"

namespace shdb
{

class FixedPage : public ITablePage
{
public:
    FixedPage(std::shared_ptr<Frame> frame, std::shared_ptr<Marshal> marshal) : frame(std::move(frame)), marshal(std::move(marshal)) { }

    RowIndex getRowCount() override { return getRowCapacity(); }

    Row getRow(RowIndex index) override
    {
        auto * row_data = frame->getData() + index * getRowSpace();
        if (static_cast<bool>(row_data[0]))
            return marshal->deserializeRow(row_data + 1);

        return Row();
    }

    void deleteRow(RowIndex index) override
    {
        auto * row_data = getRowData(index);
        *reinterpret_cast<bool *>(row_data) = false;
    }

    std::pair<bool, RowIndex> insertRow(const Row & row) override
    {
        if (auto [found, row_index] = findRowSlot(); found)
        {
            auto * row_data = getRowData(row_index);
            *reinterpret_cast<bool *>(row_data) = true;
            marshal->serializeRow(row_data + 1, row);
            return {true, row_index};
        }
        return {false, -1};
    }

private:
    size_t getRowSpace() { return 1 + marshal->getFixedRowSpace(); }

    uint8_t * getRowData(RowIndex index) { return frame->getData() + index * getRowSpace(); }

    RowIndex getRowCapacity() { return PageSize / getRowSpace(); }

    std::pair<bool, RowIndex> findRowSlot()
    {
        for (RowIndex index = 0; index < getRowCapacity(); ++index)
        {
            auto * row_data = getRowData(index);
            if (!static_cast<bool>(row_data[0]))
                return {true, index};
        }
        return {false, -1};
    }

    std::shared_ptr<Frame> frame;
    std::shared_ptr<Marshal> marshal;
};

class FixedPageProvider : public IPageProvider
{
public:
    explicit FixedPageProvider(std::shared_ptr<Marshal> marshal) : marshal(marshal) { }

    std::shared_ptr<IPage> getPage(std::shared_ptr<Frame> frame) override { return std::make_shared<FixedPage>(std::move(frame), marshal); }

    std::shared_ptr<Marshal> marshal;
};

std::shared_ptr<IPageProvider> createFixedPageProvider(std::shared_ptr<Schema> schema)
{
    auto marshal = std::make_shared<Marshal>(std::move(schema));
    return std::make_shared<FixedPageProvider>(std::move(marshal));
}

}
