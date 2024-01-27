#include "flexible_page.h"

namespace NKV {

int FlixiblePage::getByIndex(int index) {
    int answer = 0 ;
    for (int i = 3; i >= 0; --i) {
        answer += (int)fileEditor_.Get(4 * index + i);
        if (i) {
            answer <<= 4;
        }
    }
    return answer;
}

void FlixiblePage::setByIndex(int index, int value) {
    for (int i = 0; i < 4; ++i) {
        fileEditor_.Set(4 * index + i, value & ((1 << 4) - 1));
        value >>= 4;
    }
}

int FlixiblePage::getCntRows() {
    return getByIndex(0);
}

std::vector<int> FlixiblePage::getSizesRows() {
    int cnt_rows = getCntRows();
    std::vector<int> answers;
    for (int i = 0; i < cnt_rows; ++i) {
        answers.push_back(getByIndex(i + 1));
    }
    return answers;
}

void FlixiblePage::eraseLength(int index) {
    int cnt_elems = getCntRows();
    setByIndex(0, cnt_elems - 1);
    for (int i = index + 1; i < cnt_elems; ++i) {
        setByIndex(i, getByIndex(i + 1));
    }
    setByIndex(cnt_elems, 0);   
}

void FlixiblePage::eraseRow(int len, int prev_sum, int all_sum) {
    for (int i = page_size - len - prev_sum; i >= page_size - all_sum; --i) {
        setByIndex(i + len, getByIndex(i));
    }
    for (int i = page_size - all_sum; i < page_size - all_sum + len; ++i) {
        setByIndex(i, 0);
    }
}

FlixiblePage::FlixiblePage(const char* name) : fileEditor_(FileEditor(name, DISK_SIZE)) {}

int FlixiblePage::getRowCount() {
    int answer = getCntRows();
    return answer;
}

std::string FlixiblePage::getRow(int index) {
    std::vector<int> row_lenghts = getSizesRows();
    if (static_cast<size_t>(index) >= row_lenghts.size()) {
        throw std::runtime_error("out of range get row error");
    }
    int real_index = 0;
    for (int i = 0; i <= index; ++i) {
        real_index += row_lenghts[i];
    }
    std::string answer;
    for (int i = 0; i < row_lenghts[index]; ++i) {
        answer.push_back(getByIndex(page_size - real_index + i));
    }
    return answer;
}

void FlixiblePage::deleteRow(int index) {
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

std::pair<bool, int> FlixiblePage::insertRow(const std::string &row) {   
    size_t space_row = row.size();
    size_t nakop_size = 0;
    std::vector<int> row_lenghts = getSizesRows();
    for (auto elem: row_lenghts) {
        nakop_size += elem;
    }
    if (nakop_size + space_row + 2 + row_lenghts.size() > (size_t)page_size) {
        return {false, -1};
    }
    for (size_t i = 0; i < space_row; ++i) {
        setByIndex(page_size - nakop_size - space_row + i, row[i]);
    }
    setByIndex(1 + row_lenghts.size(), space_row);
    assert(getByIndex(1 + row_lenghts.size()) == static_cast<int>(space_row));
    setByIndex(0, row_lenghts.size() + 1);
    return {true, row_lenghts.size()};
}

}
