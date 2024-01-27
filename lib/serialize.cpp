#include "serialize.h"


namespace NUtils::NSerialization {

std::string SerializeValue(size_t value) {
    return std::to_string(value);
}

size_t DeserializeValue(const std::string& value, size_t startPos) {
    return std::stoul(value.substr(startPos));
}

std::vector<size_t> Deserialize(const std::string& encoded) {
    std::vector<size_t> result;
    size_t pos = 0;

    while (pos < encoded.size()) {
        size_t nextPos = encoded.find(' ', pos);
        result.push_back(DeserializeValue(encoded, pos));
        pos = (nextPos == std::string::npos) ? nextPos : nextPos + 1;
    }

    return result;
}

std::string Serialize(const std::vector<size_t>& decoded) {
    std::ostringstream oss;

    for (const auto& num : decoded) {
        oss << SerializeValue(num) << ' ';
    }

    return oss.str();
}
    
}