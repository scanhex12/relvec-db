#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <random>
#include <cassert>

#include "../vectors/vec_operations.h"
#include "../vectors/knn_searcher.h"
#include "../vectors/lsh.h"
#include "../vectors/hnsw.h"


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

template<typename T>
void test_linear_knn(T& knn, size_t n) {
    for (int i = 1; i < n; ++i) {
        auto vec = NVec::TVector({i + 0.0, 0.0, i + 0.0});
        vec.SetMeta(i);
        knn.AddVector(vec);
    }
    knn.Build();

    
    NVec::TVector query({0.0, 0.0, 0.0});
    for (size_t i = 1; i < n; ++i) {
        auto res = knn.SearchNearestK(query, i);
        REQUIRE(res.size() == i);
        std::vector<bool> used(i + 1);
        used[0] = 1;
        for (const auto& nearest_vector: res) {
            used[*nearest_vector.GetMeta()] = 1;
        }
        auto all_correct = true;
        for (auto elem: used) {
            all_correct &= elem;
        }
        REQUIRE(all_correct);
    }
}

TEST_CASE("knn_simple") {
    NKNN::SimpleKNNSearch knn1(NKNN::Metric::L2);
    test_linear_knn<NKNN::SimpleKNNSearch>(knn1, 100);
    NKNN::SimpleKNNSearch knn2(NKNN::Metric::Cosine);
    test_linear_knn<NKNN::SimpleKNNSearch>(knn2, 100);
    NKNN::SimpleKNNSearch knn3(NKNN::Metric::DotProduct);
    test_linear_knn<NKNN::SimpleKNNSearch>(knn3, 100);
}

TEST_CASE("test_lsh_simple") {
    NKNN::LSH knn1(1, 3, NKNN::Metric::L2);
    test_linear_knn<NKNN::LSH>(knn1, 100);

    NKNN::LSH knn2(10, 3, NKNN::Metric::L2);
    test_linear_knn<NKNN::LSH>(knn2, 100);

    NKNN::LSH knn3(100, 3, NKNN::Metric::L2);
    test_linear_knn<NKNN::LSH>(knn3, 100);

    NKNN::LSH knn4(1000, 3, NKNN::Metric::L2);
    test_linear_knn<NKNN::LSH>(knn4, 100);

    NKNN::LSH knn5(10, 3, NKNN::Metric::Cosine);
    test_linear_knn<NKNN::LSH>(knn5, 100);

    NKNN::LSH knn6(10, 3, NKNN::Metric::DotProduct);
    test_linear_knn<NKNN::LSH>(knn6, 100);
}


template<typename T>
void test_stress_knn(T& knn, size_t n, size_t dim) {
    for (int i = 1; i < n; ++i) {
        auto vec = GenerateFixedSize<double>(dim);
        vec.SetMeta(i);
        knn.AddVector(vec);
    }
    knn.Build();

    
    NVec::TVector query(GenerateFixedSize<double>(dim));
    for (size_t i = 1; i < n; ++i) {
        auto res = knn.SearchNearestK(query, i);
        REQUIRE(res.size() == i);
        std::vector<bool> used(i + 1);
        used[0] = 1;
        for (const auto& nearest_vector: res) {
            used[*nearest_vector.GetMeta()] = 1;
        }
        auto all_correct = true;
        for (auto elem: used) {
            all_correct &= elem;
        }
        REQUIRE(all_correct);
    }
}
/*
TEST_CASE("test_lsh_stress") {
    NKNN::LSH knn(1000, 5, NKNN::Metric::L2);
    test_stress_knn<NKNN::LSH>(knn, 100, 5);
}
*/

TEST_CASE("test_hnsw_simple") {
    NKNN::HNSW knn1(1, 3, NKNN::Metric::L2);
    test_linear_knn<NKNN::HNSW>(knn1, 1);
/*
    NKNN::HNSW knn2(10, 3, NKNN::Metric::L2);
    test_linear_knn<NKNN::HNSW>(knn2, 100);

    NKNN::HNSW knn3(100, 3, NKNN::Metric::L2);
    test_linear_knn<NKNN::HNSW>(knn3, 100);

    NKNN::HNSW knn4(1000, 3, NKNN::Metric::L2);
    test_linear_knn<NKNN::HNSW>(knn4, 100);

    NKNN::HNSW knn5(10, 3, NKNN::Metric::Cosine);
    test_linear_knn<NKNN::HNSW>(knn5, 100);

    NKNN::HNSW knn6(10, 3, NKNN::Metric::DotProduct);
    test_linear_knn<NKNN::HNSW>(knn6, 100);*/
}
