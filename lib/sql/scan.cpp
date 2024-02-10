#include "scan.h"

namespace shdb {

    ScanIterator::ScanIterator(std::shared_ptr<ITable> table,
                               PageIndex page_index,
                               RowIndex row_index) : Table_(table), PageIndex_(page_index), RowIndex_(row_index) {
    }

    RowId ScanIterator::getRowId() const {
        return RowId{page_index : PageIndex_, row_index : RowIndex_};
    }

    Row ScanIterator::getRow() {
        return Table_->getRow(getRowId());
    }

    Row ScanIterator::operator*() {
        return Table_->getRow(getRowId());
    }

    bool ScanIterator::operator==(const ScanIterator &other) const {
        return (Table_ == other.Table_ &&
        PageIndex_ == other.PageIndex_ &&
        RowIndex_ == other.RowIndex_);
    }

    bool ScanIterator::operator!=(const ScanIterator &other) const {
        return !(Table_ == other.Table_ &&
        PageIndex_ == other.PageIndex_ &&
        RowIndex_ == other.RowIndex_);
    }

    ScanIterator& ScanIterator::operator++() {
        if (RowIndex_ + 1 < Table_->getPage(PageIndex_)->getRowCount()) {
            RowIndex_++;
        } else {
            PageIndex_++;
            RowIndex_ = 0;
        }
        return *this;
    }

    Scan::Scan(std::shared_ptr<ITable> table) : Table_(table) {
    }

    ScanIterator Scan::begin() const {
        return ScanIterator(Table_, 0, 0);
    }

    ScanIterator Scan::end() const {
        int last_page_idx = Table_->getPageCount() - 1;
        return ScanIterator(Table_, last_page_idx, Table_->getPage(last_page_idx)->getRowCount() - 1);
    }
}    // namespace shdb
