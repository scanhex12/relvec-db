#include "hnsw.h"
#include <random>
#include <algorithm>


namespace NKNN {
    std::vector<NVec::TVector> HNSW::FindNearest(size_t layer, size_t k) {
            
    }
    
    HNSW::HNSW(size_t k, size_t dim, Metric metric) : k_(k), dim_(dim), metric_(metric) {}

    std::vector<NVec::TVector> HNSW::SearchNearestK(NVec::TVector vector, size_t k) {
        auto dst = [&](const NVec::TVector& a, const NVec::TVector& b) {
            if (metric_ == Metric::Cosine) {
                auto val1 = a.CosineDistanceFast(b);
                return val1;
            } else if (metric_ == Metric::L2) {
                auto val1 = a.L2DistanceFast(b);
                return val1;
            } else {
                auto val1 = a.DotProductFast(b);
                return val1;
            }
        };

        auto buffer = VertexBuffer(k);
        int startVertex = rand() % vectorLog_.size();
        buffer.Put(startVertex, dst(vectorLog_[startVertex], vector));
        while (true) {
            auto iterV = buffer.GetSmallestByV();
            int cnt_added = 0;
            for (auto u : graph_.back()[iterV]) {
                auto curdst = dst(vectorLog_[u], vector);
                cnt_added += buffer.Put(u, curdst);
            }
            if (cnt_added == 0) {
                break;
            }
        }

    }
    
    void HNSW::AddVector(NVec::TVector vector) {
        vectorLog_.push_back(vector);
    }
    
    void HNSW::Build() {
        auto dst = [&](const NVec::TVector& a, const NVec::TVector& b) {
            if (metric_ == Metric::Cosine) {
                auto val1 = a.CosineDistanceFast(b);
                return val1;
            } else if (metric_ == Metric::L2) {
                auto val1 = a.L2DistanceFast(b);
                return val1;
            } else {
                auto val1 = a.DotProductFast(b);
                return val1;
            }
        };


        size_t n = vectorLog_.size();
        std::vector<int> available;
        for (size_t i = 0; i < n; ++i) {
            available.push_back(i);
        }
        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(available.begin(), available.end(), g);
        size_t iter = 0;
        for (int layer = 0; layer < n; ++layer) {
            if (iter == available.size()) {
                break;
            }
            layers_.push_back({});
            while (iter < available.size() && layers_.back().size() < (1 << layer)) {
                layers_.back().push_back(available[iter]);
                iter++;
            }
        }
        graph_.resize(layers_.size());
        graph_[0].resize(n);
        for (size_t layer = 1; layer < layers_.size(); ++layer) {
            graph_[layer] = graph_[layer - 1];
            for (auto curV : layers_[layer]) {
                auto buffer = VertexBuffer(k_);
                int startVertex = layers_[layer - 1][0];
                buffer.Put(curV, dst(vectorLog_[curV], vectorLog_[startVertex]));
                while (true) {
                    auto iterV = buffer.GetSmallestByV();
                    int cnt_added = 0;
                    for (auto u : graph_[layer - 1][iterV]) {
                        auto curdst = dst(vectorLog_[u], vectorLog_[curV]);
                        cnt_added += buffer.Put(u, curdst);
                    }
                    if (cnt_added == 0) {
                        break;
                    }
                }
                for (auto u : buffer.GetAllVertexes()) {
                    graph_[layer][u].push_back(curV);
                    graph_[layer][curV].push_back(u);
                }
            }  
        }
    }
}
