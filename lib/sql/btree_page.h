#pragma once

#include <cstring>
#include <iostream>
#include <memory>
#include <optional>
#include <cassert>
#include <stdexcept>

#include "bufferpool.h"
#include "comparator.h"
#include "marshal.h"
#include "page.h"
#include "row.h"
#include "table.h"

namespace shdb
{

enum BTreePageType : uint32_t
{
    invalid = 0,
    metadata,
    internal,
    leaf
};

template<class T>
void serializeValue(const T &value, uint8_t *&data)
{
    auto *vdata = reinterpret_cast<T *>(data);
    *vdata++ = value;
    data = reinterpret_cast<uint8_t *>(vdata);
}


template<class T>
T deserializeValue(uint8_t *&data)
{
    auto *vdata = reinterpret_cast<T *>(data);
    auto result = *vdata++;
    data = reinterpret_cast<uint8_t *>(vdata);
    return result;
}

std::string toString(BTreePageType page_type);

class BTreePage : public IPage
{
public:
    explicit BTreePage(
        std::shared_ptr<Frame> frame_, std::shared_ptr<Marshal> marshal_, uint32_t key_size_in_bytes_, uint32_t max_page_size_)
        : frame(std::move(frame_)), marshal(std::move(marshal_)), key_size_in_bytes(key_size_in_bytes_), max_page_size(max_page_size_)
    {
    }

    static constexpr size_t HeaderOffset = sizeof(BTreePageType);

    const std::shared_ptr<Frame> & getFrame() const { return frame; }

    std::shared_ptr<Frame> & getFrame() { return frame; }

    const std::shared_ptr<Marshal> & getMarshal() const { return marshal; }

    BTreePageType getPageType() const { return getValue<BTreePageType>(0); }

    void setPageType(BTreePageType btree_page_type) { setValue(0, static_cast<uint32_t>(btree_page_type)); }

    bool isInvalidPage() const { return getPageType() == BTreePageType::invalid; }

    bool isLeafPage() const { return getPageType() == BTreePageType::leaf; }

    bool isInternalPage() const { return getPageType() == BTreePageType::internal; }

    bool isMetadataPage() const { return getPageType() == BTreePageType::metadata; }

    uint32_t getMaxPageSize() const { return max_page_size; }

    uint32_t getMinPageSize() const { return max_page_size / 2; }

    template <typename T>
    const T * getPtrValue(size_t index, size_t bytes_offset = 0) const
    {
        return reinterpret_cast<T *>(frame->getData() + bytes_offset) + index;
    }

    template <typename T>
    T * getPtrValue(size_t index, size_t bytes_offset = 0)
    {
        return reinterpret_cast<T *>(frame->getData() + bytes_offset) + index;
    }

    template <typename T>
    const T & getValue(size_t index, size_t bytes_offset = 0) const
    {
        return *getPtrValue<T>(index, bytes_offset);
    }

    template <typename T>
    T & getValue(size_t index, size_t bytes_offset = 0)
    {
        return *getPtrValue<T>(index, bytes_offset);
    }

    template <typename T>
    void setValue(size_t index, const T & value, size_t bytes_offset = 0)
    {
        getValue<T>(index, bytes_offset) = value;
    }

    const uint32_t key_size_in_bytes;
    const uint32_t max_page_size;

private:
    std::shared_ptr<Frame> frame;
    std::shared_ptr<Marshal> marshal;
};

using BTreePagePtr = std::shared_ptr<BTreePage>;

/** BTreeMetadataPage, first page in BTree index.
  * Contains necessary metadata information for btree index startup.
  *
  * Header format:
  * --------------------------------------------------------------------------
  * | PageType (4) | RootPageIndex (4) | KeySizeInBytes (4) | MaxPageSize(4) |
  * --------------------------------------------------------------------------
  */
class BTreeMetadataPage
{
public:
    explicit BTreeMetadataPage(BTreePagePtr page) : page(std::move(page)) { }

    static_assert(sizeof(PageIndex) == sizeof(uint32_t));

    static constexpr size_t RootPageIndexHeaderOffset = 0;

    static constexpr size_t KeySizeInBytesHeaderOffset = 1;

    static constexpr size_t MaxPageSizeHeaderOffset = 2;

    const BTreePagePtr & getRawPage() const { return page; }

    PageIndex getRootPageIndex() const { return page->getValue<PageIndex>(RootPageIndexHeaderOffset, BTreePage::HeaderOffset); }

    void setRootPageIndex(PageIndex root_page_index)
    {
        page->setValue(RootPageIndexHeaderOffset, root_page_index, BTreePage::HeaderOffset);
    }

    uint32_t getKeySizeInBytes() const { return page->getValue<uint32_t>(KeySizeInBytesHeaderOffset, BTreePage::HeaderOffset); }

    void setKeySizeInBytes(uint32_t key_size_in_bytes)
    {
        page->setValue(KeySizeInBytesHeaderOffset, key_size_in_bytes, BTreePage::HeaderOffset);
    }

    uint32_t getMaxPageSize() const { return page->getValue<uint32_t>(MaxPageSizeHeaderOffset, BTreePage::HeaderOffset); }

    void setMaxPageSize(uint32_t max_page_size) { page->setValue(MaxPageSizeHeaderOffset, max_page_size, BTreePage::HeaderOffset); }

    std::ostream & dump(std::ostream & stream, size_t offset = 0) const
    {
        std::string offset_string(offset, ' ');

        stream << offset_string << "Root page index " << getRootPageIndex() << '\n';
        stream << offset_string << "Key size in bytes " << getKeySizeInBytes() << '\n';
        stream << offset_string << "Max page size " << getMaxPageSize() << '\n';

        return stream;
    }

private:
    BTreePagePtr page;
};

/* BTree internal page.
 * Store N indexed keys and N + 1 child page indexes (PageIndex) within internal page.
 * First key is always invalid.
 *
 *  Header format (size in bytes, 4 * 2 = 8 bytes in total):
 *  -------------------------------
 * | PageType (4) | CurrentSize(4) |
 *  -------------------------------
 *
 * Internal page format (keys are stored in order):
 *
 *  -------------------------------------------------------------------------------------------------
 * | HEADER | INVALID_KEY(1) + PAGE_INDEX(1) | KEY(2) + PAGE_INDEX(2) | ... | KEY(n) + PAGE_INDEX(n) |
 *  -------------------------------------------------------------------------------------------------
 */
class BTreeInternalPage
{
public:
    explicit BTreeInternalPage(BTreePagePtr page_) : page(std::move(page_)) { }

    static_assert(sizeof(PageIndex) == sizeof(BTreePageType));

    static_assert(sizeof(PageIndex) == sizeof(uint32_t));

    static constexpr size_t CurrentSizeHeaderIndex = 0;

    static constexpr size_t HeaderOffset = BTreePage::HeaderOffset + sizeof(uint32_t) * (CurrentSizeHeaderIndex + 1);

    static constexpr size_t calculateMaxKeysSize(uint32_t key_size_in_bytes)
    {
        return (PageSize - HeaderOffset) / (key_size_in_bytes + sizeof(PageIndex));
    }

    const BTreePagePtr & getRawPage() const { return page; }

    uint32_t getSize() const { return page->getValue<uint32_t>(CurrentSizeHeaderIndex, BTreePage::HeaderOffset); }

    void setSize(uint32_t size) { page->setValue(CurrentSizeHeaderIndex, size, BTreePage::HeaderOffset); }

    void increaseSize(uint32_t amount) { page->getValue<uint32_t>(CurrentSizeHeaderIndex, BTreePage::HeaderOffset) += amount; }

    Row getKey(size_t index) const
    {
        size_t keyOffset = index * getEntrySize();
        auto keyPtr = page->getPtrValue<uint8_t>(keyOffset, HeaderOffset);
        return page->getMarshal()->deserializeRow(keyPtr);
    }

    void setKey(size_t index, const Row &key) {
        page->getMarshal()->serializeRow(page->getPtrValue<uint8_t>((index * getEntrySize()), HeaderOffset), key);
    }


    PageIndex getValue(size_t index) const
    {
        size_t keyOffset = (index * getEntrySize()) + page->key_size_in_bytes;
        auto keyPtr = page->getPtrValue<uint8_t>(keyOffset, HeaderOffset);
        return deserializeValue<PageIndex>(keyPtr);
    }

    /// Set value for specified index
    void setValue(size_t index, const PageIndex & value)
    {
        size_t keyOffset = (index * getEntrySize()) + page->key_size_in_bytes;
        auto keyPtr = page->getPtrValue<uint8_t>(keyOffset, HeaderOffset);
        serializeValue<int>(value, keyPtr);
    }

    /// Set key and value for specified index
    void setEntry(size_t index, const Row & key, const PageIndex & value)
    {
        size_t keyOffset = (index * getEntrySize()) + page->key_size_in_bytes;
        auto keyPtr = page->getPtrValue<uint8_t>(keyOffset, HeaderOffset);
        serializeValue<int>(value, keyPtr);
        
        keyOffset = (index * getEntrySize());
        keyPtr = page->getPtrValue<uint8_t>(keyOffset, HeaderOffset);
        page->getMarshal()->serializeRow(keyPtr, key);
    }

    /// Insert first value for invalid key
    void insertFirstEntry(const PageIndex & value)
    {
        increaseSize(1);
        setValue(0, value);
    }

    /// Insert key and value for specified index
    bool insertEntry(size_t index, const Row & key, const PageIndex & value)
    {
        if (getSize() + 1 > calculateMaxKeysSize(page->key_size_in_bytes)) {
            return false;
        }
        increaseSize(1);

        for (size_t i = getSize() - 1; i > index; --i) {
            setKey(i, getKey(i - 1));
            setValue(i, getValue(i - 1));
        }
        setValue(index, value);
        setKey(index, key);
        return true;
    }

    /// Lookup specified key in page
    std::pair<PageIndex, size_t> lookupWithIndex(const Row & key) const
    {
        //std::cout << "lookup debug start " << key << '\n';
        for (size_t i = 0; i < getSize(); ++i) {
            //std::cout << "lookup check " << i << ' ' << getKey(i) << '\n';
            if (getKey(i) == key) {
                //std::cout << "lookup debug " << i << ' ' << key << '\n';
                return {getValue(i), i};
            }
        }
        return {getValue(0), 0};
    }

    /// Lookup specified key in page
    PageIndex lookup(const Row & key) const { return lookupWithIndex(key).first; }

    /** Split current page and move top half of keys to rhs_page.
      * Return top key.
      */
    Row split(BTreeInternalPage & rhs_page)
    {
        size_t halfSize = getSize() / 2;
        int start_rhs_sz = rhs_page.getSize();
        rhs_page.increaseSize(getSize() - halfSize);
        for (size_t i = halfSize; i < getSize(); ++i) {
            assert(rhs_page.insertEntry(i - halfSize, getKey(i), getValue(i)));
            //rhs_page.setValue(start_rhs_sz + i - halfSize + 1, getValue(i));
            setValue(i, {});
        }

        setSize(halfSize);
        std::cout << "Split " << halfSize << '\n';
        //rhs_page.increaseSize(halfSize);
        return getKey(halfSize - 1);
    }

    void checkCorrectness(const std::string& key_exception) {
        for (size_t i = 1; i < getSize() - 1; ++i) {
            if (compareRows(getKey(i), getKey(i + 1)) != -1) {
                dump(std::cout);
                throw std::runtime_error(key_exception);
            }
        }
    }

    std::ostream & dump(std::ostream & stream, size_t offset = 0) const
    {
        size_t size = getSize();

        std::string offset_string(offset, ' ');
        stream << offset_string << "Size " << size << '\n';
        for (size_t i = 0; i < size; ++i)
        {
            stream << offset_string << "I " << i << " key " << (i == 0 ? "invalid" : toString(getKey(i)));
            stream << " value " << getValue(i) << '\n';
        }

        return stream;
    }

    size_t lowerBound(const Row & key) const
    {
        for (size_t i = 1; i < getSize(); ++i) {
            if (compareRows(getKey(i), key) == 0) {
                return i;
            }

            if (compareRows(getKey(i), key) == -1) {
                continue;
            }
            return i;
        }
        if (getSize() != 0) {
            return getSize();
        }
        return 0;
    }

    void removeByIndex(size_t index) {
        for (size_t i = index + 1; i < getSize(); ++i) {
            setKey(i - 1, getKey(i));
            setValue(i - 1, getValue(i));
        }
        increaseSize(-1);
    }


private:
    inline uint8_t * getEntryStartOffset(size_t index) const
    {
        uint8_t * key_ptr = page->getPtrValue<uint8_t>(0, HeaderOffset);
        size_t key_offset = getEntrySize() * index;
        return key_ptr + key_offset;
    }

    inline size_t getEntrySize() const { return page->key_size_in_bytes + sizeof(PageIndex); }

    BTreePagePtr page;
};

/** Store indexed key and value. Only support unique key.
  *
  *  Header format (size in byte, 4 * 4 = 16 bytes in total):
  *  ------------------------------------------------------------------------
  * | PageType (4) | PageSize(4) | PreviousPageIndex (4) | NextPageIndex (4) |
  *  ------------------------------------------------------------------------
  *
  *  Leaf page format (keys are stored in order):
  *  --------------------------------------------------------------------
  * | HEADER | KEY(1) + RID(1) | KEY(2) + RID(2) | ... | KEY(n) + RID(n) |
  *  --------------------------------------------------------------------
  */
class BTreeLeafPage
{
public:
    explicit BTreeLeafPage(BTreePagePtr page_) : page(std::move(page_)) { }

    static_assert(sizeof(PageIndex) == sizeof(BTreePageType));

    static_assert(sizeof(PageIndex) == sizeof(uint32_t));

    static constexpr size_t PageSizeHeaderIndex = 0;

    static constexpr size_t PreviousPageIdHeaderIndex = 1;

    static constexpr size_t NextPageIdHeaderIndex = 2;

    static constexpr size_t HeaderOffset = BTreePage::HeaderOffset + sizeof(uint32_t) * (NextPageIdHeaderIndex + 1);

    static constexpr size_t calculateMaxKeysSize(uint32_t key_size_in_bytes)
    {
        return (PageSize - HeaderOffset) / (sizeof(RowId) + key_size_in_bytes);
    }

    const BTreePagePtr & getRawPage() const { return page; }

    uint32_t getSize() const { return page->getValue<uint32_t>(PageSizeHeaderIndex, BTreePage::HeaderOffset); }

    void setSize(uint32_t size) { page->setValue(PageSizeHeaderIndex, size, BTreePage::HeaderOffset); }

    void increaseSize(uint32_t amount) { page->getValue<uint32_t>(PageSizeHeaderIndex, BTreePage::HeaderOffset) += amount; }

    PageIndex getPreviousPageIndex() const { return page->getValue<PageIndex>(PreviousPageIdHeaderIndex, BTreePage::HeaderOffset); }

    void setPreviousPageIndex(PageIndex previous_page_index)
    {
        page->setValue(PreviousPageIdHeaderIndex, previous_page_index, BTreePage::HeaderOffset);
    }

    PageIndex getNextPageIndex() const { return page->getValue<PageIndex>(NextPageIdHeaderIndex, BTreePage::HeaderOffset); }

    void setNextPageIndex(PageIndex next_page_index) { page->setValue(NextPageIdHeaderIndex, next_page_index, BTreePage::HeaderOffset); }

    Row getKey(size_t index) const
    {
        size_t keyOffset = index * getEntrySize();
        auto keyPtr = page->getPtrValue<uint8_t>(keyOffset, HeaderOffset);
        return page->getMarshal()->deserializeRow(keyPtr);
    }

    void setKey(size_t index, const Row &key) {
        page->getMarshal()->serializeRow(page->getPtrValue<uint8_t>( (index * getEntrySize()), HeaderOffset), key);
    }

    void setValue(size_t index, const RowId & value)
    {
        size_t keyOffset = (index * getEntrySize()) + page->key_size_in_bytes;
        auto keyPtr = page->getPtrValue<uint8_t>(keyOffset, HeaderOffset);
        serializeValue<int32_t>(value.page_index, keyPtr);
        keyOffset += sizeof(value.page_index);
        keyPtr = page->getPtrValue<uint8_t>(keyOffset, HeaderOffset);
        serializeValue<int32_t>(value.row_index, keyPtr);

    }

    RowId getValue(size_t index) const
    {
        size_t keyOffset = (index * getEntrySize()) + page->key_size_in_bytes;
        auto keyPtr = page->getPtrValue<uint8_t>(keyOffset, HeaderOffset);
        auto keyPtr2 = page->getPtrValue<uint8_t>(keyOffset + sizeof(int32_t), HeaderOffset);
        return RowId{deserializeValue<int32_t>(keyPtr), deserializeValue<int32_t>(keyPtr2)};
    }

    /// Insert specified key and value in page
    bool insert(const Row & key, const RowId & value)
    {
        
        if (getSize() + 1 > calculateMaxKeysSize(page->key_size_in_bytes)) {
            return false;
        }

        size_t index = 0;
        while (index < getSize() && compareRows(getKey(index), key) == -1) {
            ++index;
        }

        if (index < getSize() && getKey(index) == key) {
            throw std::invalid_argument("Duplicate key");
        }

        increaseSize(1);

        for (size_t i = getSize() - 1; i > index; --i) {
            setKey(i, getKey(i - 1));
            setValue(i, getValue(i - 1));
        }

        setKey(index, key);
        setValue(index, value);

        return true;
    }

    void checkCorrectness(const std::string& key_exception) {
        if (getSize() == 0) {
            return;
        }
        for (size_t i = 1; i < getSize() - 1; ++i) {
            if (compareRows(getKey(i), getKey(i + 1)) != -1) {
                dump(std::cout);
                throw std::runtime_error(key_exception);
            }
        }
    }


    /// Lookup specified key in page
    std::optional<RowId> lookup(const Row & key) const
    {
        for (size_t i = 0; i < getSize(); ++i) {
            if (getKey(i) == key) {
                return getValue(i);
            }
        }
        return std::nullopt;
    }

    bool insertEntry(size_t index, const Row & key, const RowId & value)
    {
        if (getSize() + 1 > calculateMaxKeysSize(page->key_size_in_bytes)) {
            return false;
        }
        increaseSize(1);

        for (size_t i = getSize() - 1; i > index; --i) {
            setKey(i, getKey(i - 1));
            setValue(i, getValue(i - 1));
        }
        
        setValue(index, value);
        setKey(index, key);

        return true;
    }

    /// Return index of lower bound for specified key
    size_t lowerBound(const Row & key) const
    {
        for (size_t i = 0; i < getSize(); ++i) {
            if (compareRows(getKey(i), key) == 0) {
                return i;
            }

            if (compareRows(getKey(i), key) == -1) {
                continue;
            }
            return i;
        }
        if (getSize() != 0) {
            return getSize();
        }
        return 0;
    }

    /// Remove specified key from page
    bool remove(const Row & key)
    {
        size_t index = 0;
        while (index < getSize() && compareRows(getKey(index), key) != 0) {
            ++index;
        }

        if (index == getSize()) {
            return false;
        }

        for (size_t i = index; i < getSize() - 1; ++i) {
            setKey(i, getKey(i + 1));
            setValue(i, getValue(i + 1));
        }

        increaseSize(-1);

        return true;
    }

    /** Split current page and move top half of keys to rhs_page.
      * Return top key.
      */
    Row split(BTreeLeafPage & rhs_page)
    {
        rhs_page.checkCorrectness("check correctness right before split");
        checkCorrectness("check correctness left before split");
        size_t halfSize = getSize() / 2;
        rhs_page.increaseSize(getSize() - halfSize);
        for (int i = rhs_page.getSize() - 1; i >= 0; --i) {
            rhs_page.setKey(i + getSize() - halfSize, rhs_page.getKey(i));
            rhs_page.setValue(i + getSize() - halfSize, rhs_page.getValue(i));
        }
        for (size_t i = halfSize; i < getSize(); ++i) {
            rhs_page.setKey(i - halfSize, getKey(i));
            rhs_page.setValue(i - halfSize, getValue(i));
        }
        rhs_page.checkCorrectness("check correctness right after split");

        setSize(halfSize);
        return getKey(halfSize - 1);
    }


    std::vector<Row> merge(BTreeLeafPage & rhs_page)
    {
        if (rhs_page.getSize() == 0) {
            throw std::runtime_error("empty merge right");
        }
        if (compareRows(getKey(getSize() - 1), rhs_page.getKey(0)) != -1) {
            std::cout << "LEFT TABLE\n";
            dump(std::cout);
            std::cout << "RIGHT TABLE\n";
            rhs_page.dump(std::cout);
            throw std::runtime_error("incorrect usage of merge right");
        }
        size_t start_size = getSize();
        increaseSize(rhs_page.getSize());
        std::vector<Row> answer;
        for (size_t i = 0; i < rhs_page.getSize(); ++i) {
            answer.push_back(rhs_page.getKey(i));
            setKey(i + start_size, rhs_page.getKey(i));
            setValue(i + start_size, rhs_page.getValue(i));
        }
        //rhs_page.setSize(0);
        return answer;
    }

    std::vector<Row> mergeLeft(BTreeLeafPage & rhs_page)
    {
        if (rhs_page.getSize() == 0) {
            throw std::runtime_error("empty merge left");
        }

        if (compareRows(getKey(getSize() - 1), rhs_page.getKey(0)) != 1) {
            std::cout << "LEFT TABLE\n";
            dump(std::cout);
            std::cout << "RIGHT TABLE\n";
            rhs_page.dump(std::cout);
            throw std::runtime_error("incorrect usage of merge left");
        }

        size_t start_size = getSize();
        increaseSize(rhs_page.getSize());
        for (int i = getSize() - 1; i >= 0; --i) {
            setKey(i + rhs_page.getSize(), getKey(i));
            setValue(i + rhs_page.getSize(), getValue(i));
        }
        std::vector<Row> answer;

        for (size_t i = 0; i < rhs_page.getSize(); ++i) {
            answer.push_back(rhs_page.getKey(i));
            setKey(i, rhs_page.getKey(i));
            setValue(i, rhs_page.getValue(i));
        }
        rhs_page.setSize(0);
        return answer;
    }



    std::ostream & dump(std::ostream & stream, size_t offset = 0) const
    {
        size_t size = getSize();

        std::string offset_string(offset, ' ');
        stream << offset_string << "Size " << size << '\n';

        auto previous_page_index = getPreviousPageIndex();
        auto next_page_index = getNextPageIndex();

        stream << "Previous page index " << (previous_page_index == InvalidPageIndex ? "invalid" : std::to_string(previous_page_index))
               << '\n';
        stream << "Next page index " << (next_page_index == InvalidPageIndex ? "invalid" : std::to_string(next_page_index)) << '\n';

        for (size_t i = 0; i < size; ++i)
        {
            stream << offset_string << "I " << i << " key " << getKey(i);
            stream << " value " << getValue(i) << '\n';
        }

        return stream;
    }

private:
    inline uint8_t * getEntryStartOffset(size_t index) const
    {
        uint8_t * key_ptr = page->getPtrValue<uint8_t>(0, HeaderOffset);
        size_t key_offset = getEntrySize() * index;
        return key_ptr + key_offset;
    }

    inline size_t getEntrySize() const { return page->key_size_in_bytes + sizeof(RowId); }

    BTreePagePtr page;
};

std::shared_ptr<IPageProvider>
createBTreePageProvider(std::shared_ptr<Marshal> marshal, uint32_t key_size_in_bytes, uint32_t max_page_size);

}
