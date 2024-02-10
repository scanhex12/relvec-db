#pragma once

#include <cstdint>
#include <stdexcept>
#include "btree_page.h"
#include "database.h"
#include "page.h"
#include "row.h"
#include "table.h"

namespace shdb
{

class BTree;
using BTreePtr = std::shared_ptr<BTree>;

class BTreeIndexTable
{
public:
    BTreeIndexTable() = default;

    explicit BTreeIndexTable(std::shared_ptr<IIndexTable> table_) : table(std::move(table_)) { }

    void setIndexTable(std::shared_ptr<IIndexTable> index_table) { table = std::move(index_table); }

    PageIndex getPageCount() { return table->getPageCount(); }

    std::pair<BTreeMetadataPage, RowIndex> allocateMetadataPage()
    {
        PageIndex page_index = allocatePage();
        auto raw_page = getPage(page_index);
        raw_page->setPageType(BTreePageType::metadata);

        return {BTreeMetadataPage(raw_page), page_index};
    }

    BTreeMetadataPage getMetadataPage(PageIndex page_index) { return BTreeMetadataPage(getPage(page_index)); }

    std::pair<BTreeLeafPage, RowIndex> allocateLeafPage()
    {
        PageIndex page_index = allocatePage();
        auto raw_page = getPage(page_index);
        raw_page->setPageType(BTreePageType::leaf);

        return {BTreeLeafPage(raw_page), page_index};
    }

    BTreeLeafPage getLeafPage(PageIndex page_index) { return BTreeLeafPage(getPage(page_index)); }

    std::pair<BTreeInternalPage, RowIndex> allocateInternalPage()
    {
        PageIndex page_index = allocatePage();
        auto raw_page = getPage(page_index);
        raw_page->setPageType(BTreePageType::internal);

        return {BTreeInternalPage(raw_page), page_index};
    }

    BTreeInternalPage getInternalPage(PageIndex page_index) { return BTreeInternalPage(getPage(page_index)); }

    inline BTreePagePtr getPage(PageIndex page_index) { return std::static_pointer_cast<BTreePage>(table->getPage(page_index)); }

private:
    inline PageIndex allocatePage() { return table->allocatePage(); }

    std::shared_ptr<IIndexTable> table;
};

class BTree : public IIndex
{
public:
    static BTreePtr createIndex(const IndexMetadata & index_metadata, Store & store);

    static BTreePtr createIndex(const IndexMetadata & index_metadata, size_t page_max_keys_size, Store & store);

    static void removeIndex(const std::string & name_, Store & store);

    static void removeIndexIfExists(const std::string & name_, Store & store);

    void insert(const IndexKey & index_key, const RowId & row_id) override;

    bool remove(const IndexKey & index_key, const RowId & row_id) override;

    bool remove_with_ignore_limitations(const IndexKey & index_key);

    void checkPageIndex(PageIndex ind, std::string key_throw) {
        if (ind == InvalidPageIndex) {
            return;
        }
        std::cout << "START_CHECK\n";
        auto leaf = index_table.getLeafPage(ind);
        if (leaf.getPreviousPageIndex() == InvalidPageIndex) {
            std::cout << "END_CHECK\n";
            return;
        }
        auto prev_leaf = index_table.getLeafPage(leaf.getPreviousPageIndex());
        if (prev_leaf.getNextPageIndex() != ind) {
            std::cout << prev_leaf.getNextPageIndex() << ' ' << ind  << ' ' << leaf.getPreviousPageIndex() << ' ' << InvalidPageIndex << '\n';
            throw std::runtime_error(key_throw + " incorrect prev -> next");
        }
        if (leaf.getNextPageIndex() == InvalidPageIndex) {
            std::cout << "END_CHECK\n";
            return;
        }
        auto next_leaf = index_table.getLeafPage(leaf.getNextPageIndex());
        if (next_leaf.getPreviousPageIndex() != ind) {
            std::cout << next_leaf.getPreviousPageIndex() << ' ' << ind << ' ' << InvalidPageIndex << '\n';
            throw std::runtime_error(key_throw + " incorrect next -> prev");
        }
        std::cout << "END_CHECK\n";
    }


    void lookup(const IndexKey & index_key, std::vector<RowId> & result) override;

    std::unique_ptr<IIndexIterator> read() override;

    std::unique_ptr<IIndexIterator> read(const KeyConditions & predicates) override;

    size_t getMaxPageSize() const { return max_page_size; }

    const BTreeIndexTable & getIndexTable() const { return index_table; }

    BTreeIndexTable & getIndexTable() { return index_table; }

    void dump(std::ostream & stream);

    static constexpr PageIndex MetadataPageIndex = 0;

private:
    BTreeLeafPage lookupLeafPage(const IndexKey & index_key);

    BTreeLeafPage lookupLeftmostLeafPage();

    BTree(const IndexMetadata & metadata_, Store & store, std::optional<size_t> page_max_keys_size);

    size_t max_page_size = 0;

    BTreeIndexTable index_table;

    BTreeMetadataPage metadata_page;

};

}
