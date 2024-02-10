#pragma once

#include "schema.h"
#include "table.h"

namespace shdb
{

std::shared_ptr<IPageProvider> createFixedPageProvider(std::shared_ptr<Schema> schema);

}
