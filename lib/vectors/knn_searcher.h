#pragma once

#include "vec_operations.h"


namespace NKNN {
    enum Metric {
        Cosine,
        L2,
        DotProduct
    };

    class IKNNSearch {
    public:
        virtual std::vector<NVec::TVector> SearchNearestK(NVec::TVector vector, size_t k) = 0;
        virtual void AddVector(NVec::TVector vector) = 0;
        virtual void Build() = 0;
    };

    class SimpleKNNSearch : public IKNNSearch {
        std::vector<NVec::TVector> results;
        Metric metric_;
    public:

        SimpleKNNSearch() = default;
        SimpleKNNSearch(Metric metric);

        std::vector<NVec::TVector> SearchNearestK(NVec::TVector vector, size_t k) override;
        void AddVector(NVec::TVector vector) override;
        void Build() override;
    };
}
