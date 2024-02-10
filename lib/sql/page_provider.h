#pragma once

#include <cstdint>
#include <functional>
#include <memory>

namespace shdb
{

class IPage;
class Frame;

class IPageProvider
{
public:
    virtual ~IPageProvider() = default;

    virtual std::shared_ptr<IPage> getPage(std::shared_ptr<Frame> frame) = 0;
};

}
