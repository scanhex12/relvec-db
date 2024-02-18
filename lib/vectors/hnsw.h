#pragma once

#include "knn_searcher.h"
#include "vec_operations.h"
#include <unordered_map>
#include <deque>
#include <set>


namespace NKNN {
    struct VertexInfo {
        int v;
        int d;

        VertexInfo(int vertex, int distance) : v(vertex), d(distance) {}

        bool operator<(const VertexInfo& other) const {
            if (d != other.d)
                return d < other.d;
            return v < other.v;
        }
        bool operator==(const VertexInfo& other) const {
            return v == other.v && d == other.d;
        }


    };

    class VertexBuffer {
        int k;
        std::unordered_map<int, int> vertexMap;
        std::set<VertexInfo> vertices;

    public:
        VertexBuffer(int maxVertices) : k(maxVertices) {}

        bool Put(int v, int d) {
            if (vertexMap.find(v) != vertexMap.end())
                return false;

            vertexMap[v] = d;
            vertices.insert(VertexInfo(v, d));

            if (vertices.size() > k) {
                int smallestV = vertices.rbegin()->v;
                vertices.erase(*vertices.rbegin());
                vertexMap.erase(smallestV);
            }
            return true;
        }

        std::vector<int> GetAllVertexes() {
            std::vector<int> allVertexes;
            for (const auto& pair : vertexMap) {
                allVertexes.push_back(pair.first);
            }
            return allVertexes;
        }

        int GetSmallestByV() {
            return vertices.begin()->v;
        }
    };

    class HNSW : public IKNNSearch {
        Metric metric_;
        size_t k_; // vertex degree
        size_t dim_;

        std::vector<NVec::TVector> vectorLog_;
        std::vector<std::vector<int>> layers_;
        std::vector<std::vector<std::vector<int>>> graph_; // T, V, [.. edges]

        std::vector<NVec::TVector> FindNearest(size_t layer, size_t k);
    public:
        HNSW(size_t k, size_t dim, Metric metric);

        std::vector<NVec::TVector> SearchNearestK(NVec::TVector vector, size_t k) override;
        void AddVector(NVec::TVector vector) override;
        void Build() override;
    };
}
