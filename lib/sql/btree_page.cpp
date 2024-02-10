#include "btree_page.h"

namespace shdb
{

namespace
{

class BTreePageProvider : public IPageProvider
{
public:
    explicit BTreePageProvider(std::shared_ptr<Marshal> marshal_, uint32_t key_size_in_bytes_, uint32_t max_page_size_)
        : marshal(std::move(marshal_)), key_size_in_bytes(key_size_in_bytes_), max_page_size(max_page_size_)
    {
    }

    std::shared_ptr<IPage> getPage(std::shared_ptr<Frame> frame) override
    {
        return std::make_shared<BTreePage>(frame, marshal, key_size_in_bytes, max_page_size);
    }

    const uint32_t key_size_in_bytes;

    const uint32_t max_page_size;

private:
    std::shared_ptr<Marshal> marshal;
};

}

std::string toString(BTreePageType page_type)
{
    switch (page_type)
    {
        case invalid:
            return "Invalid";
        case metadata:
            return "Metadata";
        case internal:
            return "Internal";
        case leaf:
            return "Leaf";
    }

    return {};
}

std::shared_ptr<IPageProvider> createBTreePageProvider(std::shared_ptr<Marshal> marshal, uint32_t key_size_in_bytes, uint32_t max_page_size)
{
    return std::make_shared<BTreePageProvider>(std::move(marshal), key_size_in_bytes, max_page_size);
}



}
