#include "flexible.h"

#include "marshal.h"
#include "page.h"
#include "row.h"
#include "table.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>
#include <variant>

namespace shdb {

// Your code goes here.

    class FlixiblePage :
            public ITablePage {


        std::shared_ptr<Frame> frame;
        std::shared_ptr<Marshal> marshal;

        uint8_t *getRowData(RowIndex index)
        {
            return frame->getData() + index;
        }

        uint8_t getByIndex(int index) {
            return (int)*(frame->getData() + index);   
        }

        void setByIndex(int index, int value) {
            if (index < 0) {
                return;
            }
            auto *row_data = getRowData(index);
            row_data[0] = value;
        }

        int getCntRows() {
            return getByIndex(0);
        }

        std::vector<int> getSizesRows() {
            int cnt_rows = getCntRows();
            std::vector<int> answers;
            for (int i = 0; i < cnt_rows; ++i) {
                answers.push_back(getByIndex(i + 1));
            }
            return answers;
        }

        void eraseLength(int index) {
            int cnt_elems = getCntRows();
            setByIndex(0, cnt_elems - 1);
            for (int i = index + 1; i < cnt_elems; ++i) {
                setByIndex(i, getByIndex(i + 1));
            }
            setByIndex(cnt_elems, 0);   
        }

        void eraseRow(int len, int prev_sum, int all_sum) {
            if (prev_sum == 0) {
                return;
            }
            for (int i = PageSize - len - prev_sum; i >= PageSize - all_sum; --i) {
                setByIndex(i + len, getByIndex(i));
            }
            for (int i = PageSize - all_sum; i < PageSize - all_sum + len; ++i) {
                setByIndex(i, 0);
            }
        }

    public:
        FlixiblePage(std::shared_ptr<Frame> frame, std::shared_ptr<Marshal> marshal)
                :
                frame(std::move(frame)), marshal(std::move(marshal)) {}

        RowIndex getRowCount()  {
            return PageSize - 1;
        }

        Row getRow(RowIndex index)  {
            std::vector<int> row_lenghts = getSizesRows();
            if (static_cast<size_t>(index) >= row_lenghts.size() || row_lenghts[index] == 0) {
                return Row();
            }
            int real_index = 0;
            for (int i = 0; i <= index; ++i) {
                real_index += row_lenghts[i];
            }
            auto *row_data = getRowData(PageSize - real_index);
            auto row = marshal->deserializeRow(row_data);
            return row;
        }

        void deleteRow(RowIndex index) {
            std::vector<int> row_lenghts = getSizesRows();
            int len = row_lenghts[index];
            int prev_sum = 0;
            for (int i = 0; i < index; ++i) {
                prev_sum += row_lenghts[i];
            }
            int all_sum = 0;
            for (auto elem: row_lenghts) {
                all_sum += elem;
            }
            setByIndex(index + 1, 0);
            eraseRow(len, prev_sum, all_sum);
        }

        std::pair<bool, RowIndex> insertRow(const Row &row) {
            size_t space_row = marshal->getRowSpace(row);
            size_t nakop_size = 0;
            std::vector<int> row_lenghts = getSizesRows();
            for (auto elem: row_lenghts) {
                nakop_size += elem;
            }
            if (nakop_size + space_row + 2 + row_lenghts.size() > PageSize) {
                return {false, -1};
            }
            auto *row_data = getRowData(PageSize - nakop_size - space_row);
            marshal->serializeRow(row_data, row);
            setByIndex(1 + row_lenghts.size(), space_row);
            assert(getByIndex(1 + row_lenghts.size()) == static_cast<int>(space_row));
            setByIndex(0, row_lenghts.size() + 1);

            return {true, row_lenghts.size()};
        }
    };


class FlexiblePageProvider : public IPageProvider
{
public:
    explicit FlexiblePageProvider(std::shared_ptr<Marshal> marshal) : marshal(marshal) { }

    std::shared_ptr<IPage> getPage(std::shared_ptr<Frame> frame) override { return std::make_shared<FlixiblePage>(std::move(frame), marshal); }

    std::shared_ptr<Marshal> marshal;
};

std::shared_ptr<IPageProvider> createFlexiblePageProvider(std::shared_ptr<Schema> schema)
{
    auto marshal = std::make_shared<Marshal>(std::move(schema));

    return std::make_shared<FlexiblePageProvider>(std::move(marshal));
}

}    // namespace shdb
