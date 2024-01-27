#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../lib/serialize.h"

#include <random>
#include <iostream>

static constexpr size_t TEST_CASES_SERIALIZE = 10;

void SerializeValueHelper(size_t value) {
    auto encoded = NUtils::NSerialization::SerializeValue(value);
    auto decoded = NUtils::NSerialization::DeserializeValue(encoded, 0);
    REQUIRE(value == decoded);
}

void SerializeVectorHelper(const std::vector<size_t>& values) {
    auto encoded = NUtils::NSerialization::Serialize(values);
    auto decoded = NUtils::NSerialization::Deserialize(encoded);
    if (values != decoded) {
        for (auto elem: values) {
            std::cerr << elem << '\n';
        }
    }
    REQUIRE(values == decoded);
}

std::vector<size_t> GenerateVector() {
    size_t n = 1 + rand() % 1000;
    std::vector<size_t> values;
    for (size_t i = 0; i < n; ++i) {
        size_t val = rand() % 100000;
        values.push_back(val);
    }
    return values;
}

TEST_CASE("test_serialize_value") {
    for (size_t i = 0; i < 10000; ++i) {
        SerializeValueHelper(i);
    }
}

TEST_CASE("test_serialize_vector_simple") {
    SerializeVectorHelper({});
    SerializeVectorHelper({1});
    SerializeVectorHelper({1,2,3});
    SerializeVectorHelper({4,5,6});
    SerializeVectorHelper({123});
    SerializeVectorHelper({1,2,3,4,5});
    SerializeVectorHelper({101, 213, 412, 512, 661, 3123});
}

TEST_CASE("test_serialize_vector_random") {
    for (size_t test_case = 0; test_case < TEST_CASES_SERIALIZE; ++test_case) {
        SerializeVectorHelper(std::move(GenerateVector()));
    }
}
