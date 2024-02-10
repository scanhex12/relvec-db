#pragma once

#include "row.h"
#include <stdexcept>

namespace shdb
{

/** The comparator functor returns -1, 0, or 1 based on the comparison of lhs and rhs rows.
  * Returns -1 if |lhs| is lexicographically less than |rhs|.
  * Returns 0 if both rows are equal.
  * Returns 1 if |lhs| is lexicographically greater than |rhs|.
  */
struct Comparator
{
    int16_t operator()(const Row & lhs, const Row & rhs, std::vector<bool> desc = {}) const;
};

/// Compare rows using Comparator
int16_t compareRows(const Row & lhs, const Row & rhs);

/// Compare value using Comparator
int16_t compareValue(const Value & lhs, const Value & rhs);
}
