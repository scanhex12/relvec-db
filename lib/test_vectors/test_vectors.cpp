#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <random>
#include <cassert>

#include "../vectors/vec_operations.h"

#define ASSERT_EQ(a, b) REQUIRE(a == b)
#define EPS 1e-9
#define ASSERT_EQ_REAL(a, b) REQUIRE(std::fabs(a - b) < EPS)

static constexpr size_t TEST_CASES = 100;

template<typename T>
NVec::TVector GenerateFixedSize(size_t n) {
    std::vector<T> values;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (size_t i = 0; i < n; ++i) {
        values.push_back(dis(gen));
    }
    return NVec::TVector(values);
}


template<typename T>
NVec::TVector Generate() {
    size_t n = rand() % 100 + 1;
    return GenerateFixedSize<T>(n);
}


TEST_CASE("vec_simple") {
    NVec::TVector a({0.5, 0.1});
    NVec::TVector b({0.2, 0.4});

    ASSERT_EQ_REAL(a.DotProductNaive(b), 0.14);
    ASSERT_EQ_REAL(a.L2DistanceNaive(b), 2 * 0.3 * 0.3);

    NVec::TVector c({1.0, 0.0});
    NVec::TVector d({0.0, 1.0});

    ASSERT_EQ_REAL(c.CosineDistanceNaive(d), 1.0);
}

TEST_CASE("vec_simd_dot_float") {
    for (size_t iter = 0; iter < TEST_CASES; ++iter) {
        auto vec1 = Generate<float>();
        auto vec2 = GenerateFixedSize<float>(vec1.Size());
        ASSERT_EQ(vec1.Size(), vec2.Size());
        ASSERT_EQ_REAL(vec1.DotProductNaive(vec2), vec1.CosineDistanceFast(vec2));
    }    
}

TEST_CASE("vec_simd_dot_double") {
    for (size_t iter = 0; iter < TEST_CASES; ++iter) {
        auto vec1 = Generate<double>();
        auto vec2 = GenerateFixedSize<double>(vec1.Size());
        ASSERT_EQ(vec1.Size(), vec2.Size());
        ASSERT_EQ_REAL(vec1.DotProductNaive(vec2), vec1.CosineDistanceFast(vec2));
    }    
}

TEST_CASE("vec_simd_dot_long_double") {
    for (size_t iter = 0; iter < TEST_CASES; ++iter) {
        auto vec1 = Generate<long double>();
        auto vec2 = GenerateFixedSize<long double>(vec1.Size());
        ASSERT_EQ(vec1.Size(), vec2.Size());
        ASSERT_EQ_REAL(vec1.DotProductNaive(vec2), vec1.CosineDistanceFast(vec2));
    }
}
