#pragma once

#include "knn_searcher.h"

namespace NKNN {

    class IndexLine : public IKNNSearch{
        std::vector<std::pair<double, NVec::TVector>> line_;
        NVec::TVector vector_;
    public:
        IndexLine() = delete;
        IndexLine(size_t dim);

        std::vector<NVec::TVector> SearchNearestK(NVec::TVector vector, size_t k) override;
        void AddVector(NVec::TVector vector) override;
        void Build() override;
    };

    class LSH : public IKNNSearch {
        std::vector<IndexLine> indices_;
        Metric metric_;
    public:
        LSH(size_t k, size_t dim, Metric metric);

        std::vector<NVec::TVector> SearchNearestK(NVec::TVector vector, size_t k) override;
        void AddVector(NVec::TVector vector) override;
        void Build() override;
    };
}
