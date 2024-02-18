#include "knn_searcher.h"
#include "vec_operations.h"
#include <algorithm>


namespace NKNN {

SimpleKNNSearch::SimpleKNNSearch(Metric metric) : metric_(metric) {}

std::vector<NVec::TVector> SimpleKNNSearch::SearchNearestK(NVec::TVector vector, size_t k) {
    auto cmp = [&](const NVec::TVector& a, const NVec::TVector& b) {
        if (metric_ == Metric::Cosine) {
            auto val1 = vector.CosineDistanceFast(a);
            auto val2 = vector.CosineDistanceFast(b);
            return val1 < val2;
        } else if (metric_ == Metric::L2) {
            auto val1 = vector.L2DistanceFast(a);
            auto val2 = vector.L2DistanceFast(b);
            return val1 < val2;
        } else {
            auto val1 = vector.DotProductFast(a);
            auto val2 = vector.DotProductFast(b);
            return val1 < val2;
        }
    };

    std::partial_sort(results.begin(), results.begin() + k, results.end(), cmp);
    std::vector<NVec::TVector> answer;
    for (size_t i = 0; i < k; ++i) {
        answer.push_back(results[i]);
    }
    return answer;
}

void SimpleKNNSearch::AddVector(NVec::TVector vector) {
    results.push_back(vector);
}

void SimpleKNNSearch::Build() {}



}