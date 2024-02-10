#include "btree.h"
#include "btree_page.h"
#include "comparator.h"
#include "page.h"
#include "row.h"

#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <stdexcept>
#include <vector>

namespace shdb
{

BTree::BTree(const IndexMetadata & metadata_, Store & store_, std::optional<size_t> page_max_keys_size)
    : IIndex(metadata_), metadata_page(nullptr)
{
    if (!page_max_keys_size)
    {
        size_t internal_page_max_keys_size = BTreeInternalPage::calculateMaxKeysSize(metadata.fixedKeySizeInBytes());
        size_t leaf_page_max_keys_size = BTreeLeafPage::calculateMaxKeysSize(metadata.fixedKeySizeInBytes());
        page_max_keys_size = std::min(internal_page_max_keys_size, leaf_page_max_keys_size);
    }

    max_page_size = *page_max_keys_size;
    auto page_provider = createBTreePageProvider(metadata.getKeyMarshal(), metadata.fixedKeySizeInBytes(), max_page_size);
    index_table.setIndexTable(store_.createOrOpenIndexTable(metadata.getIndexName(), page_provider));

    bool initial_index_creation = index_table.getPageCount() == 0;

    if (initial_index_creation)
    {
        auto [allocated_metadata_page, metadata_page_index] = index_table.allocateMetadataPage();
        //assert(metadata_page_index == MetadataPageIndex);

        auto [root_page, root_page_index] = index_table.allocateLeafPage();

        root_page.setPreviousPageIndex(InvalidPageIndex);
        root_page.setNextPageIndex(InvalidPageIndex);

        metadata_page = std::move(allocated_metadata_page);
        metadata_page.setRootPageIndex(root_page_index);
        metadata_page.setMaxPageSize(max_page_size);
        metadata_page.setKeySizeInBytes(metadata.fixedKeySizeInBytes());
        //std::cout << "RESETED METADATA!\n";
        return;
    }

    metadata_page = index_table.getMetadataPage(MetadataPageIndex);

    if (metadata.fixedKeySizeInBytes() != metadata_page.getKeySizeInBytes())
        throw std::runtime_error(
            "BTree index inconsistency. Expected " + std::to_string(metadata_page.getKeySizeInBytes()) + " key size in bytes. Actual "
            + std::to_string(metadata.fixedKeySizeInBytes()));

    if (max_page_size != metadata_page.getMaxPageSize())
        throw std::runtime_error(
            "BTree index inconsistency. Expected " + std::to_string(metadata_page.getMaxPageSize()) + " max page size. Actual "
            + std::to_string(max_page_size));
}



void BTree::insert(const IndexKey & index_key, const RowId & row_id)
{
    //std::cout << "INSERTION!!! " << index_key.size() << ' ' << row_id.page_index << ' ' << row_id.row_index << '\n';
    auto iter = index_table.getPage(metadata_page.getRootPageIndex());
    auto type = iter->getPageType();
    std::vector<BTreePagePtr> stack;
    std::vector<size_t> indices;
    while (true) {
        if (iter->getPageType() == BTreePageType::metadata) {
            auto metadata = BTreeMetadataPage(iter);
            iter = index_table.getPage(metadata.getRootPageIndex());
            //assert(iter->getPageType() != BTreePageType::metadata);
            continue;
        } else if (iter->getPageType() == BTreePageType::internal) {
            stack.push_back(iter);
            auto vertex = BTreeInternalPage(iter);
            size_t lower_bound_indx = vertex.lowerBound(index_key);
            if (lower_bound_indx == vertex.getSize()) {
                lower_bound_indx--;   
            }
            size_t lower_bound_page = vertex.getValue(lower_bound_indx);
            indices.push_back(lower_bound_indx);
            //std::cout << "Internal page insertion key " << toString(index_key) << ' ' << lower_bound_indx << '\n';
            //vertex.dump(std::cout);

            auto child = index_table.getPage(lower_bound_page);
            iter = child;
            continue;
        } else if (iter->getPageType() == BTreePageType::leaf) {
            auto leaf_vertex = BTreeLeafPage(iter);
            size_t index = leaf_vertex.lowerBound(index_key);
            //std::cout << "CHECK "<< index << ' ' << index_key.size() << '\n';
            //leaf_vertex.checkCorrectness("check leaf in beginning before insertion");

            bool inserted = leaf_vertex.insertEntry(index,index_key, row_id);
            //std::cout << "OK INSERTION!!!\n";
            if (!inserted) {
                //leaf_vertex.checkCorrectness("check leaf in beginning");
                //std::cout << "ALLOCATION!!!\n";
                bool resolved = false;
                auto allocated_leaf = index_table.allocateLeafPage();
                auto new_leaf_node = allocated_leaf.first;


                new_leaf_node.setNextPageIndex(leaf_vertex.getNextPageIndex());
                leaf_vertex.setNextPageIndex(allocated_leaf.second);

                if (!stack.empty()) {
                    auto cur_id = BTreeInternalPage(stack.back()).getValue(indices.back());
                    new_leaf_node.setPreviousPageIndex(cur_id);
                    //std::cout << "check ids " << cur_id << ' ' << allocated_leaf.second << '\n';
                } else {
                    //throw std::runtime_error("stack is empty!");                    
                }
                //checkPageIndex(leaf_vertex.getNextPageIndex(), "check next");
                //checkPageIndex(leaf_vertex.getPreviousPageIndex(), "check prev");
                //checkPageIndex(cur_id, "check prev");


                auto max_val_left = leaf_vertex.split(new_leaf_node);//should upd
                //assert(leaf_vertex.insert(index_key, row_id));
                
                if (compareRows(max_val_left, index_key) == -1) {

                    (new_leaf_node.insertEntry(new_leaf_node.lowerBound(index_key), index_key, row_id));
                } else {
                    (leaf_vertex.insertEntry(leaf_vertex.lowerBound(index_key), index_key, row_id));

                }
                max_val_left = leaf_vertex.getKey(leaf_vertex.getSize() - 1);
                auto max_val_right = new_leaf_node.getKey(new_leaf_node.getSize() - 1);

                if (compareRows(max_val_left, max_val_right) != -1) {
                    /*
                    std::cout << "RIGHT (allocated) LEAF\n";
                    allocated_leaf.first.dump(std::cout);
                    std::cout << "LEFT LEAF\n";
                    leaf_vertex.dump(std::cout);
                    std::cout << "LEFT MAX " <<toString(max_val_left) << ' '<< "RIGHT MAX " <<toString(max_val_right) << '\n';
                    throw std::runtime_error("incorrect keys");*/
                }
                auto left = leaf_vertex.getRawPage();//should upd
                auto right = new_leaf_node.getRawPage();//should upd
                RowIndex right_row_index = allocated_leaf.second;//should upd
                RowIndex left_row_index = metadata_page.getRootPageIndex();//should upd
                //std::cout << "(BREAK POINT 1) " << stack.size() << '\n';

                while (!stack.empty()) {
                    auto ptr = BTreeInternalPage(stack.back());
                    stack.pop_back();
                    left_row_index = ptr.getValue(indices.back());
                    ptr.setKey(indices.back(), max_val_left);
                    //ptr.checkCorrectness("check corr key 1");
                    //ptr.dump(std::cout);
                    if (ptr.insertEntry(indices.back() + 1, max_val_right, right_row_index)) {
                        //std::cout << "inserted key " << indices.back() << ' ' << toString(max_val_right) << '\n';
                        //ptr.checkCorrectness("check corr key 2");
                        resolved = true;
                        break;
                    }
                    throw std::runtime_error("ni error!");
                    /*
                    indices.pop_back();
                    auto allocated_page = index_table.allocateInternalPage();
                    allocated_page.first.insertFirstEntry(-1);
                    max_val_left = ptr.split(allocated_page.first);
                    left = ptr.getRawPage();
                    right = allocated_page.first.getRawPage();
                    right_row_index = allocated_page.second;*/
                }
                if (!resolved) {
                    auto allocated_page = index_table.allocateInternalPage();
                    auto internal_page = allocated_page.first;
                    internal_page.insertFirstEntry(left_row_index);
                    internal_page.insertEntry(1, max_val_left, left_row_index);
                    internal_page.insertEntry(2, max_val_right, right_row_index);

                    auto left_leaf = index_table.getLeafPage(left_row_index);
                    auto right_leaf = index_table.getLeafPage(right_row_index);
                    //std::cout << "RELINKING! " << left_leaf.getSize() << ' ' << right_leaf.getSize() << '\n';

                    right_leaf.setPreviousPageIndex(left_row_index);
                    left_leaf.setPreviousPageIndex(InvalidPageIndex);

                    left_leaf.setNextPageIndex(right_row_index);
                    right_leaf.setNextPageIndex(InvalidPageIndex);
                    //right_row_index = allocated_page.second;
                    
                    //int index_insert = internal_page.lowerBound(max_val_left);
                    // вчера остановился здесь, нужно правильно прилинковать left и right к internal_page
                    
                    //assert(internal_page.insertEntry(index_insert + 1, max_val_right, right_row_index));
                    //internal_page.setKey(index_insert, max_val_left);

                    //internal_page.dump(std::cout);
                    //std::cout << "SETTED ROOT INDEX " << allocated_page.second << '\n';
                    //std::cout << "LEAFS INDEX " << left_row_index << ' ' << right_row_index << '\n';

                    metadata_page.setRootPageIndex(allocated_page.second);
                    //std::cout << "GET " << metadata_page.getRootPageIndex() << '\n';
                    
                }
            }
            break;
        } else {
            throw std::runtime_error("Invalid iter type");
        }
    }
}

bool BTree::remove_with_ignore_limitations(const IndexKey & index_key) {
    auto iter = index_table.getPage(metadata_page.getRootPageIndex());
    auto type = iter->getPageType();
    std::vector<BTreePagePtr> stack;
    std::vector<size_t> indices;
    while (true) {
        if (iter->getPageType() == BTreePageType::metadata) {
            auto metadata = BTreeMetadataPage(iter);
            iter = index_table.getPage(metadata.getRootPageIndex());
            continue;
        } else if (iter->getPageType() == BTreePageType::internal) {
            stack.push_back(iter);
            auto vertex = BTreeInternalPage(iter);
            size_t lower_bound_indx = vertex.lowerBound(index_key);
            if (lower_bound_indx == vertex.getSize()) {
                lower_bound_indx--;   
            }
            size_t lower_bound_page = vertex.getValue(lower_bound_indx);
            indices.push_back(lower_bound_indx);
            auto child = index_table.getPage(lower_bound_page);
            iter = child;
            continue;
        } else if (iter->getPageType() == BTreePageType::leaf) {
            auto leaf_vertex = BTreeLeafPage(iter);

            //leaf_vertex.checkCorrectness("fictive correctness remove check start");

            bool removed = leaf_vertex.remove(index_key);
            //leaf_vertex.checkCorrectness("fictive correctness remove check");
            if (!removed) {
                return false;
            }

            if (stack.empty()) {
                return true;
            }

            if (leaf_vertex.getSize() == 0) {
                {
                    //std::cout << "REORDER START\n";
                    auto next_leaf_ind = leaf_vertex.getNextPageIndex();
                    auto prev_leaf_ind = leaf_vertex.getPreviousPageIndex();
                    //std::cout << "REORDER " << prev_leaf_ind << ' ' << next_leaf_ind << '\n';
                    if (prev_leaf_ind != InvalidPageIndex) {
                        auto prev_leaf = index_table.getLeafPage(prev_leaf_ind);
                        prev_leaf.setNextPageIndex(next_leaf_ind);
                    }
                    if (next_leaf_ind != InvalidPageIndex) {
                        auto next_leaf = index_table.getLeafPage(next_leaf_ind);
                        next_leaf.setPreviousPageIndex(prev_leaf_ind);
                    }
                    //std::cout << "REORDER END\n";
                } 
                while (!stack.empty()) {
                    auto parent_ptr = BTreeInternalPage(stack.back());
                    parent_ptr.removeByIndex(indices.back());
                    //std::cout << "DUMP IN WHILE " << stack.size() << '\n';
                    //parent_ptr.dump(std::cout);

                    indices.pop_back();
                    stack.pop_back();
                    if (parent_ptr.getSize() > 1) {
                        break;
                    }
                    if (parent_ptr.getSize() == 1) {
                        parent_ptr.removeByIndex(0);
                        if (stack.empty()) {
                            
                            //auto [allocated_metadata_page, metadata_page_index] = index_table.allocateMetadataPage();
                            //assert(metadata_page_index == MetadataPageIndex);

                            auto [root_page, root_page_index] = index_table.allocateLeafPage();

                            root_page.setPreviousPageIndex(InvalidPageIndex);
                            root_page.setNextPageIndex(InvalidPageIndex);

                            metadata_page = index_table.getMetadataPage(BTree::MetadataPageIndex);
                            metadata_page.setRootPageIndex(root_page_index);
                            metadata_page.setMaxPageSize(max_page_size);
                            metadata_page.setKeySizeInBytes(metadata.fixedKeySizeInBytes());

                            //std::cout << "RESETED METADATA!\n";
                        }                       
                    }
                }
                return true;
            }
            return true;
        }
    }
}

bool BTree::remove(const IndexKey & index_key, const RowId &)
{
    return remove_with_ignore_limitations(index_key);
    auto iter = index_table.getPage(metadata_page.getRootPageIndex());
    auto type = iter->getPageType();
    std::vector<BTreePagePtr> stack;
    std::vector<size_t> indices;
    while (true) {
        if (iter->getPageType() == BTreePageType::metadata) {
            auto metadata = BTreeMetadataPage(iter);
            iter = index_table.getPage(metadata.getRootPageIndex());
            continue;
        } else if (iter->getPageType() == BTreePageType::internal) {
            stack.push_back(iter);
            auto vertex = BTreeInternalPage(iter);
            size_t lower_bound_indx = vertex.lowerBound(index_key);
            if (lower_bound_indx == vertex.getSize()) {
                lower_bound_indx--;   
            }
            size_t lower_bound_page = vertex.getValue(lower_bound_indx);
            indices.push_back(lower_bound_indx);
            auto child = index_table.getPage(lower_bound_page);
            iter = child;
            continue;
        } else if (iter->getPageType() == BTreePageType::leaf) {
            auto leaf_vertex = BTreeLeafPage(iter);
            //std::cout << "REMOVE LEAF " << toString(index_key) << ' ' << leaf_vertex.getRawPage()->getMinPageSize() << ' ' <<  leaf_vertex.getSize()<< '\n';
            //leaf_vertex.checkCorrectness("correctness remove check start");

            bool removed = leaf_vertex.remove(index_key);
            //leaf_vertex.dump(std::cout);
            //leaf_vertex.checkCorrectness("correctness remove check");
            if (!removed) {
                return false;
            }

            if (stack.empty()) {
                return true;
            }

            if (leaf_vertex.getSize() == 0) {
                while (!stack.empty()) {
                    auto parent_ptr = BTreeInternalPage(stack.back());
                    parent_ptr.removeByIndex(indices.back());
                    if (parent_ptr.getSize() > 1) {
                        break;
                    }
                }
                //std::cout << "END REMOVE\n";
                return true;
            }

            if (leaf_vertex.getSize() >= leaf_vertex.getRawPage()->getMinPageSize()) {
                //std::cout << "END REMOVE\n";
                return true;
            }

            auto next_page_index = leaf_vertex.getNextPageIndex();
            if (next_page_index != InvalidPageIndex) {
                //throw std::runtime_error("A");
                auto next_page = BTreeLeafPage(index_table.getPage(next_page_index));
                auto parent_ptr = BTreeInternalPage(stack.back());

                auto keys_to_remove = leaf_vertex.merge(next_page);

                for (auto elem : keys_to_remove) {
                    //std::cout << "OK DELETED!\n";
                    remove_with_ignore_limitations(elem);
                }
                
                leaf_vertex.setNextPageIndex(next_page.getNextPageIndex());
                leaf_vertex.checkCorrectness("correctness remove right");
                //std::cout << "END REMOVE 1\n";
                return true;
            }
/*
            auto parent_ptr = BTreeInternalPage(stack.back());
            if (indices.back() == 0) {
                std::cout << "END REMOVE 2\n";
                return true;
            }
            auto prev_page_index = leaf_vertex.getPreviousPageIndex();
            auto prev_page = BTreeLeafPage(index_table.getPage(prev_page_index));
            auto cur_row_index = prev_page.getNextPageIndex();
            auto keys_to_remove = leaf_vertex.mergeLeft(prev_page);
            for (auto elem : keys_to_remove) {
                remove_with_ignore_limitations(elem);
            }
            if (prev_page.getPreviousPageIndex() != InvalidPageIndex) {
                auto prev_prev_page = BTreeLeafPage(index_table.getPage(prev_page.getPreviousPageIndex()));
                prev_prev_page.setNextPageIndex(cur_row_index);
            }

            leaf_vertex.checkCorrectness("correctness remove left");

            std::cout << "END REMOVE 3\n";*/
            return true;
        }
    }
}

void BTree::lookup(const IndexKey & index_key, std::vector<RowId> & result)
{
    //std::cout << "LOOKUP " << toString(index_key) << '\n';
    auto iter = index_table.getPage(metadata_page.getRootPageIndex());
    auto type = iter->getPageType();
    while (true) {
        if (iter->getPageType() == BTreePageType::metadata) {

            auto metadata = BTreeMetadataPage(iter);
            iter = index_table.getPage(metadata.getRootPageIndex());
            //(iter->getPageType() != BTreePageType::metadata);
            continue;
        } else if (iter->getPageType() == BTreePageType::internal) {

            auto vertex = BTreeInternalPage(iter);
            size_t lb_indx = vertex.lowerBound(index_key);
            if (lb_indx == vertex.getSize()) {
                lb_indx--;   
            }
            size_t lower_bound_page = vertex.getValue(lb_indx);
            //std::cout << "LOWER BOUND INDEX " << lb_indx << ' ' << lower_bound_page << '\n';
            //vertex.dump(std::cout);

            auto child = index_table.getPage(lower_bound_page);
            iter = child;
            continue;
        } else if (iter->getPageType() == BTreePageType::leaf) {
            auto leaf_vertex = BTreeLeafPage(iter);
            //leaf_vertex.dump(std::cout);
            size_t index = leaf_vertex.lowerBound(index_key);
            if (index >= leaf_vertex.getSize()) {
                return;
            }
            //leaf_vertex.checkCorrectness("corr lookup get");
            auto answer = leaf_vertex.getValue(index);
            //std::cout << "lookup answer " << leaf_vertex.lowerBound(index_key) << ' ' << answer << '\n';
            //leaf_vertex.checkCorrectness("corr lookup after get");

            //leaf_vertex.dump(std::cout);

            //std::cout << "READED " << answer.page_index << ' ' << answer.row_index << '\n';
            if (leaf_vertex.getKey(index) != index_key) {
                return;
            }
            result.push_back(answer);
            return;
        }
    }
}

namespace
{

class BTreeEmptyIndexIterator : public IIndexIterator
{
public:
    std::optional<std::pair<IndexKey, RowId>> nextRow() override { return {}; }
};

class BTreeIndexIterator : public IIndexIterator
{
public:
    explicit BTreeIndexIterator(
        BTreeIndexTable & index_table_,
        BTreeLeafPage leaf_page_,
        KeyConditions predicates_
        /*
        size_t leaf_page_offset_,
        std::optional<Row> max_key,
        bool include_max_key_ = false*/)
        : index_table(index_table_)
        , leaf_page(leaf_page_)
        , predicates(predicates_)
        /*
        , leaf_page_offset(leaf_page_offset_)
        , max_key(std::move(max_key))
        , include_max_key(include_max_key_)*/{};

    std::optional<std::pair<IndexKey, RowId>> nextRow() override {
        //std::cout << "READ START\n";

        while (true) {
            if (is_end) {
                return std::nullopt;
            }
            //std::cout << leaf_page_offset << ' ' << leaf_page.getSize() << '\n';
            if (leaf_page_offset >= leaf_page.getSize()) {
                return std::nullopt;
            }
            auto cur_value = leaf_page.getValue(leaf_page_offset);
            auto cur_key = leaf_page.getKey(leaf_page_offset);
            //std::cout << "READ FROM HERE\n";
            bool should_return = isRowValid(cur_key);
            leaf_page_offset++;
            if (leaf_page_offset >= leaf_page.getSize()) {
                auto next_page_index = leaf_page.getNextPageIndex();
                if (next_page_index == InvalidPageIndex) {
                    is_end = true;
                } else {
                    leaf_page = index_table.getLeafPage(next_page_index);
                    leaf_page_offset = 0;
                }
            }
            if (should_return) {
                return std::pair{cur_key, cur_value};
            }

        }
    }

private:

/*
enum class IndexComparator
{
    equal,
    notEqual,
    greater,
    greaterOrEqual,
    less,
    lessOrEqual,
};


*/

    size_t getIndexColumn(const std::string& column_name) {
        auto schema = leaf_page.getRawPage()->getMarshal()->schema;
        for (size_t i = 0; i < schema->size(); ++i) {
            if (schema->at(i).name == column_name) {
                return i;
            }
        }
        throw std::runtime_error("unknown column " + column_name);
    }

    bool isRowValid(const Row & key)
    {
        for (auto predicate : predicates) {
            if (predicate.comparator == IndexComparator::equal && compareRows({predicate.value}, {key[getIndexColumn(predicate.column.name)]}) != 0) {
                return false;
            }
            if (predicate.comparator == IndexComparator::notEqual && compareRows({predicate.value}, {key[getIndexColumn(predicate.column.name)]}) == 0) {
                return false;
            }
            if (predicate.comparator == IndexComparator::greater && compareRows({predicate.value}, {key[getIndexColumn(predicate.column.name)]}) != -1) {
                return false;
            }
            if (predicate.comparator == IndexComparator::greaterOrEqual && compareRows({predicate.value}, {key[getIndexColumn(predicate.column.name)]}) == 1) {
                return false;
            }
            if (predicate.comparator == IndexComparator::less && compareRows({predicate.value}, {key[getIndexColumn(predicate.column.name)]}) != 1) {
                return false;
            }
            if (predicate.comparator == IndexComparator::lessOrEqual && compareRows({predicate.value}, {key[getIndexColumn(predicate.column.name)]}) == -1) {
                return false;
            }
        }
        return true;
    }

    BTreeIndexTable & index_table;
    BTreeLeafPage leaf_page;
    size_t leaf_page_offset = 0;
    /*
    std::optional<Row> max_key;
    bool include_max_key;
    Row previous_row;
    */
    KeyConditions predicates;
    bool is_end = false;
};

}

std::unique_ptr<IIndexIterator> BTree::read()
{
    auto root_page_index = metadata_page.getRootPageIndex();
    auto current_page = index_table.getPage(root_page_index);
    while (!current_page->isLeafPage())
    {
        //ASSERT_TRUE(current_page->isInternalPage());
        shdb::BTreeInternalPage internal_page(current_page);
        current_page = index_table.getPage(internal_page.getValue(0));
    }

    //std::vector<std::pair<shdb::Row, shdb::RowId>> entries;

    shdb::BTreeLeafPage leaf_page(current_page);

    return std::make_unique<BTreeIndexIterator>(BTreeIndexIterator(index_table, leaf_page, {}));
}

std::unique_ptr<IIndexIterator> BTree::read(const KeyConditions & predicates)
{
    auto root_page_index = metadata_page.getRootPageIndex();
    auto current_page = index_table.getPage(root_page_index);
    while (!current_page->isLeafPage())
    {
        //ASSERT_TRUE(current_page->isInternalPage());
        shdb::BTreeInternalPage internal_page(current_page);
        current_page = index_table.getPage(internal_page.getValue(0));
    }

    //std::vector<std::pair<shdb::Row, shdb::RowId>> entries;

    shdb::BTreeLeafPage leaf_page(current_page);

    return std::make_unique<BTreeIndexIterator>(BTreeIndexIterator(index_table, leaf_page, predicates));

}

void BTree::dump(std::ostream & stream)
{
    PageIndex pages_count = index_table.getPageCount();
    for (PageIndex i = 0; i < pages_count; ++i)
    {
        auto page = index_table.getPage(i);
        auto page_type = page->getPageType();

        stream << "Page " << i << " page type " << toString(page_type) << '\n';

        switch (page_type)
        {
            case BTreePageType::invalid: {
                break;
            }
            case BTreePageType::metadata: {
                auto metadata_page = BTreeMetadataPage(page);
                metadata_page.dump(stream);
                break;
            }
            case BTreePageType::internal: {
                auto internal_page = BTreeInternalPage(page);
                internal_page.dump(stream);
                break;
            }
            case BTreePageType::leaf: {
                auto leaf_page = BTreeLeafPage(page);
                leaf_page.dump(stream);
                break;
            }
        }
    }
}

BTreeLeafPage BTree::lookupLeafPage(const IndexKey & index_key)
{
    (void)(index_key);
    throw std::runtime_error("Not implemented");
}

BTreeLeafPage BTree::lookupLeftmostLeafPage()
{
    throw std::runtime_error("Not implemented");
}

BTreePtr BTree::createIndex(const IndexMetadata & index_metadata, Store & store)
{
    return std::shared_ptr<BTree>(new BTree(index_metadata, store, {}));
}

BTreePtr BTree::createIndex(const IndexMetadata & index_metadata, size_t page_max_keys_size, Store & store)
{
    return std::shared_ptr<BTree>(new BTree(index_metadata, store, page_max_keys_size));
}

void BTree::removeIndex(const std::string & name_, Store & store)
{
    store.removeTable(name_);
}

void BTree::removeIndexIfExists(const std::string & name_, Store & store)
{
    store.removeTableIfExists(name_);
}

}
