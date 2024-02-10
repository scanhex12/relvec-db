#include "comparator.h"

#include <cassert>
#include <cstdint>
#include <string>
#include <variant>
#include <algorithm>
#include "row.h"

namespace shdb
{

template<typename T>
int16_t compareInstances(Value a, Value b) {
    if (std::holds_alternative<T>(a)) {
        assert(std::holds_alternative<T>(b));
        if (std::get<T>(a) < std::get<T>(b)) {
            return -1;
        } else if (std::get<T>(a) > std::get<T>(b)) {
            return 1;
        }
        return 0;
    }
    return 2;
}

int16_t Comparator::operator()(const Row & lhs, const Row & rhs, std::vector<bool> desc) const
{
    assert(lhs.size() == rhs.size());
    for (size_t i = 0; i < lhs.size() && i < rhs.size(); ++i) {
        bool is_reversed = desc.empty() ? false : desc[i];
        int16_t mul_coef = is_reversed ? -1 : 1;
        if (std::holds_alternative<Null>(lhs[i])) {
            assert(std::holds_alternative<Null>(rhs[i]));
            continue;
        }
        if (compareInstances<bool>(lhs[i], rhs[i]) % 2 !=0) {
            return mul_coef * compareInstances<bool>(lhs[i], rhs[i]);
        }
        if (compareInstances<int64_t>(lhs[i], rhs[i]) % 2 !=0) {
            return mul_coef * compareInstances<int64_t>(lhs[i], rhs[i]);
        }

        if (compareInstances<uint64_t>(lhs[i], rhs[i]) % 2 !=0) {
            return mul_coef * compareInstances<uint64_t>(lhs[i], rhs[i]);
        }
        if (compareInstances<std::string>(lhs[i], rhs[i]) % 2 !=0) {
            return mul_coef * compareInstances<std::string>(lhs[i], rhs[i]);
        }

    }
    return 0;
}

int16_t compareRows(const Row & lhs, const Row & rhs)
{
    return Comparator()(lhs, rhs);
}

int16_t compareValue(const Value & lhs, const Value & rhs)
{
    (void)(lhs);
    (void)(rhs);
    throw std::runtime_error("Not implemented");
}
}
