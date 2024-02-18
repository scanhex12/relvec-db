#include "lsh.h"
#include "vec_operations.h"
#include <random>
#include <algorithm>


namespace NKNN {

IndexLine::IndexLine(size_t dim) {
    std::vector<double> values;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (size_t i = 0; i < dim; ++i) {
        values.push_back(dis(gen));
    }
    vector_ = NVec::TVector(values);
}

void IndexLine::AddVector(NVec::TVector vector) {
    line_.push_back({vector_.DotProductNaive(vector), vector});
}

void IndexLine::Build() {
    std::sort(line_.begin(), line_.end(), [] (const std::pair<double, NVec::TVector>& a, const std::pair<double, NVec::TVector>& b) {
        return a.first < b.first;
    });
}

std::vector<NVec::TVector> IndexLine::SearchNearestK(NVec::TVector vector, size_t k) {
    double proj = vector_.DotProductNaive(vector);
    std::pair<double, NVec::TVector> search = {proj, vector};    
    size_t pos = std::lower_bound(line_.begin(),line_.end(),search) - line_.begin();
    
    size_t start = pos - std::min(pos, k);
    size_t end = std::min(line_.size() - 1, pos + 1);

    size_t pos_opt = 0;
    double min_distance = 1e14;

    for (size_t i = start; i <= end; ++i) {
        size_t left = i;
        size_t right = std::min(line_.size() - 1, i + k - 1);
        if (right - left + 1 < k) {
            continue;
        }
        double max_distance = std::max(std::fabs(line_[left].first - proj), std::fabs(line_[right].first - proj));
        if (max_distance < min_distance) {
            pos_opt = i;
            min_distance = max_distance;
        }
    }

    std::vector<NVec::TVector> answer;
    for (size_t i = pos_opt; i < pos_opt + k && i < line_.size(); ++i) {
        answer.push_back(line_[i].second);
    }
    return answer;
}

LSH::LSH(size_t k, size_t dim, Metric metric):metric_(metric) {
    for (size_t i = 0; i < k; ++i) {
        indices_.push_back(IndexLine(dim));
    }   
}

std::vector<NVec::TVector> LSH::SearchNearestK(NVec::TVector vector, size_t k) {
    std::vector<NVec::TVector> res;
    for (size_t i = 0; i < indices_.size(); ++i) {
        auto vecs = indices_[i].SearchNearestK(vector, k);
        for (const auto& elem: vecs) {
            res.push_back(std::move(elem));
        }
    }
    SimpleKNNSearch knn;
    for (auto elem: res) {
        knn.AddVector(elem);
    }
    knn.Build();
    return knn.SearchNearestK(vector, k);
}

void LSH::AddVector(NVec::TVector vector) {
    for (size_t i = 0; i < indices_.size(); ++i) {
        indices_[i].AddVector(vector);
    }
}

void LSH::Build() {
    for (size_t i = 0; i < indices_.size(); ++i) {
        indices_[i].Build();
    }

}


}