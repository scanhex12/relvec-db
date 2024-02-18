#pragma once

#include <vector>
#include <variant>
#include <optional>
#include <initializer_list>


namespace NVec {

    class TVector {
    public:
        using double32_t = float;
        using double64_t = double;
        using double128_t = long double;
        using VecType = std::variant<double32_t, double64_t, double128_t>;
    private:
        std::vector<double32_t> values32_;
        std::vector<double64_t> values64_;
        std::vector<double128_t> values128_;

        std::optional<size_t> metadata_;
        bool ValidateValues(const VecType& a, const VecType& b) const;
        bool Validate() const;
    public:
        enum FloatType {
            Float32,
            Float64,
            Float128
        };

        FloatType getType() const;

        TVector() = default;
        TVector(size_t n);
        TVector(const std::vector<double32_t>& values);
        TVector(const std::vector<double64_t>& values);
        TVector(const std::vector<double128_t>& values);
        TVector(const std::initializer_list<double32_t>& values);
        TVector(const std::initializer_list<double64_t>& values);
        TVector(const std::initializer_list<double128_t>& values);

        bool operator<(const TVector& a) const;

        size_t Size() const;
        void SetMeta(std::optional<size_t> metadata);
        std::optional<size_t> GetMeta() const;

        long double DotProductNaive(const TVector& a) const;
        long double DotProductFast(const TVector& a) const;

        long double L2DistanceNaive(const TVector& a) const;
        long double L2DistanceFast(const TVector& a) const;

        long double CosineDistanceNaive(const TVector& a) const;
        long double CosineDistanceFast(const TVector& a) const;

        //void SerializeVector(uint8_t *&data);
    };

    //TVector DeserializeVector(uint8_t *&data);
}
