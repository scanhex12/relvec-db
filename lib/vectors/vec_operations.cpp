#include "vec_operations.h"
#include <initializer_list>
#include <variant>
#include <immintrin.h>
#include <cmath>
#include <stdexcept>
#include <cassert>
#include <iostream>


namespace NVec {

bool TVector::ValidateValues(const VecType& a, const VecType& b) const {
    if (std::holds_alternative<double32_t>(a)) {
        return std::holds_alternative<double32_t>(b);
    } else if (std::holds_alternative<double64_t>(a)) {
        return std::holds_alternative<double64_t>(b);
    } else if (std::holds_alternative<double128_t>(a)) {
        return std::holds_alternative<double128_t>(b);
    }
}

bool TVector::Validate() const {
    int cnt = 0;
    cnt += (values32_.size() > 0);
    cnt += (values64_.size() > 0);
    cnt += (values128_.size() > 0);
    return cnt == 1;
}

TVector::TVector(size_t n) {
    values32_.resize(n);
}

TVector::TVector(const std::vector<double32_t>& values) {
    values32_.reserve(values.size());
    for (auto elem : values) {
        values32_.push_back(elem);
    }
}

TVector::TVector(const std::vector<double64_t>& values) {
    values64_.reserve(values.size());
    for (auto elem : values) {
        values64_.push_back(elem);
    }
}

TVector::TVector(const std::vector<double128_t>& values) {
    values128_.reserve(values.size());
    for (auto elem : values) {
        values128_.push_back(elem);
    }
}

TVector::TVector(const std::initializer_list<double32_t>& values) {
    values32_.reserve(values.size());
    for (auto elem : values) {
        values32_.push_back(elem);
    }
}

TVector::TVector(const std::initializer_list<double64_t>& values) {
    values64_.reserve(values.size());
    for (auto elem : values) {
        values64_.push_back(elem);
    }
}

TVector::TVector(const std::initializer_list<double128_t>& values) {
    values128_.reserve(values.size());
    for (auto elem : values) {
        values128_.push_back(elem);
    }
}


size_t TVector::Size() const {
    assert(Validate());
    return values32_.size() + values64_.size() + values128_.size();
}

void TVector::SetMeta(std::optional<size_t> metadata) {
    metadata_ = metadata;
}

std::optional<size_t> TVector::GetMeta() const {
    return metadata_;
}

TVector::FloatType TVector::getType() const {
    if (values32_.size() > 0) {
        return FloatType::Float32;
    } else if (values64_.size() > 0) {
        return FloatType::Float64;
    } else if (values128_.size() > 0) {
        return FloatType::Float128;
    }
    return FloatType::Float32;
}

long double TVector::DotProductNaive(const TVector& a) const {
    if (a.Size() != Size()) {
        throw std::runtime_error("Size of vectors should be equal");
    }
    if (a.getType() != getType()) {
        throw std::runtime_error("Vectors should have same precision");
    }
    long double result = 0.0;
    for (size_t i = 0; i < Size(); ++i) {
        if (getType() == FloatType::Float32) {
            result += values32_[i] * a.values32_[i];
        } else if (getType() == FloatType::Float64) {
            result += values64_[i] * a.values64_[i];
        } else if (getType() == FloatType::Float128) {
            result += values128_[i] * a.values128_[i];
        }
    }

    return result;
}

long double TVector::L2DistanceNaive(const TVector& a) const {
    if (a.Size() != Size()) {
        throw std::runtime_error("Size of vectors should be equal");
    }
    if (a.getType() != getType()) {
        throw std::runtime_error("Vectors should have same precision");
    }
    long double result = 0.0;
    for (size_t i = 0; i < Size(); ++i) {
        long double cur_res = 0;
        if (getType() == FloatType::Float32) {
            cur_res = (values32_[i] - a.values32_[i]);
        } else if (getType() == FloatType::Float64) {
            cur_res = (values64_[i] - a.values64_[i]);
        } else if (getType() == FloatType::Float128) {
            cur_res = (values128_[i] - a.values128_[i]);
        }
        result += cur_res * cur_res;
    }
    return result;
}


long double TVector::DotProductFast(const TVector& a) const {
    if (a.Size() != Size()) {
        throw std::runtime_error("Size of vectors should be equal");
    }
    if (a.getType() != getType()) {
        throw std::runtime_error("Vectors should have same precision");
    }
    long double result = 0.0;
    auto size = a.Size();
    if (getType() == FloatType::Float32) {
        alignas(64) double simdResult[4] = {0};

        size_t i = 0;
        for (; i + 4 <= size; i += 4) {
            __m256d vec1 = _mm256_loadu_pd(reinterpret_cast<const double*>(&values32_[i]));
            __m256d vec2 = _mm256_loadu_pd(reinterpret_cast<const double*>(&values32_[i]));
            __m256d prod = _mm256_mul_pd(vec1, vec2);
            _mm256_store_pd(&simdResult[0], prod);
            result += simdResult[0] + simdResult[1] + simdResult[2] + simdResult[3];
        }

        for (; i < size; ++i) {
            result += values32_[i] * a.values32_[i];
        }

        return result;
    }
    if (getType() == FloatType::Float64 ) {
        alignas(32) double simdResult[4] = {0};

        size_t i = 0;
        for (; i + 4 <= size; i += 4) {
            __m256d vec1 = _mm256_loadu_pd(reinterpret_cast<const double*>(&values64_[i]));
            __m256d vec2 = _mm256_loadu_pd(reinterpret_cast<const double*>(&a.values64_[i]));
            __m256d prod = _mm256_mul_pd(vec1, vec2);
            _mm256_store_pd(&simdResult[0], prod);
            result += simdResult[0] + simdResult[1] + simdResult[2] + simdResult[3];
        }

        for (; i < size; ++i) {
            result += values64_[i] * a.values64_[i];
        }

        return result;
    }
    throw std::runtime_error("your type of vector doesn't support simd instructions");
}

long double TVector::L2DistanceFast(const TVector& a) const {
    if (a.Size() != Size()) {
        throw std::runtime_error("Size of vectors should be equal");
    }
    if (a.getType() != getType()) {
        throw std::runtime_error("Vectors should have same precision");
    }
    long double result = 0.0;
    auto size = a.Size();
    if (getType() == FloatType::Float32) {
        
        alignas(32) double simdResult[4] = {0};

        size_t i = 0;
        for (; i + 4 <= size; i += 4) {
            __m256d vec1 = _mm256_loadu_pd(reinterpret_cast<const double*>(&values32_[i]));
            __m256d vec2 = _mm256_loadu_pd(reinterpret_cast<const double*>(&a.values32_[i]));
            __m256d diff = _mm256_sub_pd(vec1, vec2);
            __m256d prod = _mm256_mul_pd(diff, diff);
            _mm256_store_pd(&simdResult[0], prod);
            result += simdResult[0] + simdResult[1] + simdResult[2] + simdResult[3];
        }

        for (; i < size; ++i) {
            result += (values32_[i] - a.values32_[i]) * (values32_[i] - a.values32_[i]);
        }

        return result;
    }
    if (getType() == FloatType::Float64) {
        alignas(32) double simdResult[4] = {0};

        size_t i = 0;
        for (; i + 4 <= size; i += 4) {
            __m256d vec1 = _mm256_loadu_pd(reinterpret_cast<const double*>(&values64_[i]));
            __m256d vec2 = _mm256_loadu_pd(reinterpret_cast<const double*>(&a.values64_[i]));
            __m256d diff = _mm256_sub_pd(vec1, vec2);
            __m256d prod = _mm256_mul_pd(diff, diff);
            _mm256_store_pd(&simdResult[0], prod);
            result += simdResult[0] + simdResult[1] + simdResult[2] + simdResult[3];
        }

        for (; i < size; ++i) {
            result += (values64_[i] - a.values64_[i]) * (values64_[i] - a.values64_[i]);
        }

        return result;
    }

    if (getType() == FloatType::Float128) {
        throw std::runtime_error("your type of vector doesn't support simd instructions");
    }

    return std::sqrt(result);
}

long double TVector::CosineDistanceNaive(const TVector& a) const {
    long double dotProduct = DotProductNaive(a);
    long double normThis = std::sqrt(DotProductNaive(*this));
    long double normA = std::sqrt(a.DotProductNaive(a));

    return 1 - (dotProduct / (normThis * normA));
}

long double TVector::CosineDistanceFast(const TVector& a) const {
    long double dotProduct = DotProductFast(a);
    long double normThis = std::sqrt(DotProductNaive(*this));
    long double normA = std::sqrt(a.DotProductNaive(a));
    return 1 - (dotProduct / (normThis * normA));
}

bool TVector::operator<(const TVector& a) const {
    return false;
}

int64_t TVector::CalcHash() const {
    int64_t hsh = 0;
    for (auto elem: values32_) {
        hsh ^= static_cast<int64_t>(elem * 1000000);
    }
    for (auto elem: values64_) {
        hsh ^= static_cast<int64_t>(elem * 1000000);
    }
    for (auto elem: values128_) {
        hsh ^= static_cast<int64_t>(elem * 1000000);
    }
    return hsh;
}

bool TVector::operator==(const TVector& b) const {
    return metadata_ == b.metadata_;
}

}
