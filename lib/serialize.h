#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace NUtils::NSerialization {
    static constexpr size_t BLOCK_SIZE = 32;

    std::string SerializeValue(size_t value);
    size_t DeserializeValue(const std::string& value, size_t startPos);

    std::vector<size_t> Deserialize(const std::string& encoded);
    std::string Serialize(const std::vector<size_t>& decoded);

}