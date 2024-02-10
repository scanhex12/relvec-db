#include "statistics.h"

#include <sstream>

namespace shdb
{

std::string toString(const Statistics & statistics)
{
    std::stringstream stream;
    stream << "{page_read: " << statistics.page_read << ", "
           << "page_written: " << statistics.page_written << ", "
           << "page_accessed: " << statistics.page_accessed << "}";
    return stream.str();
}

}
