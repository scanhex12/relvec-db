#include "balancer.h"
#include <mutex>
#include <sstream>

Balancer::Balancer(const char* filename) : KVStorage_(filename) {
}

std::string Balancer::SerializeChunk(ChunkInfo info) {
    return info.first + "#" + std::to_string(info.second);
}

Balancer::ChunkInfo Balancer::DeserializeChunk(std::string serialized) {
    ChunkInfo result;
    std::size_t pos = serialized.find('#');
    if (pos != std::string::npos) {
        result.first = serialized.substr(0, pos); 
        result.second = std::stoi(serialized.substr(pos + 1));
    } else {
        result.first = "";
        result.second = 0;
    }
    return result;
}

std::string Balancer::SerializeValue(const std::vector<std::pair<Balancer::ChunkInfo, int>>& data) {
    std::ostringstream oss;
    for (const auto& pair : data) {
        oss << pair.first.first << ',' << pair.first.second << ':' << pair.second << ';';
    }
    return oss.str();
}

std::vector<std::pair<Balancer::ChunkInfo, int>> Balancer::DeserializeValue(const std::string& str) {
    std::vector<std::pair<Balancer::ChunkInfo, int>> result;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, ';')) {
        std::istringstream ss(token);
        std::string chunkName;
        int chunkValue, intValue;
        char delimiter;
        if (std::getline(ss, chunkName, ',') &&
            ss >> chunkValue >> delimiter &&
            delimiter == ':' &&
            ss >> intValue) {
            result.push_back({{chunkName, chunkValue}, intValue});
        }
    }
    return result;
}

void Balancer::OnUpdate(ChunkInfo chunk, size_t freeBytes) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto prevSize = chunk2Free_[chunk];
    freePositions_.erase({prevSize, chunk});
    chunk2Free_[chunk] = freeBytes;
    freePositions_.insert({freeBytes, chunk});
    std::cout << freePositions_.size() << '\n';
}

void Balancer::InsertTable(std::string table, size_t table_size) {
    std::cout << "balancer log " << table << ' ' << table_size << '\n';
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::pair<Balancer::ChunkInfo, int>> chunks;
    while (table_size > 0) {
        std::cout << "balancer log " << ' ' << freePositions_.size() << table << ' ' << table_size << '\n';
        if (freePositions_.size() == 0) {
            throw std::runtime_error("Not enough chunks in balancer to insert table");
        }
        auto last = *freePositions_.rbegin();
        if (table_size >= last.first) {
            table_size -= last.first;
            freePositions_.erase(last);
            chunks.push_back({last.second, last.first});
        } else {
            std::cout << "start size " << freePositions_.size() << '\n';
            freePositions_.erase(last);
            std::cout << "mid size " << freePositions_.size() << '\n';
            last.first -= table_size;
            freePositions_.insert(last);
            std::cout << "last size " << freePositions_.size() << '\n';
            chunks.push_back({last.second, table_size});
            table_size = 0;
        }
    }
    KVStorage_.set(table, SerializeValue(chunks));

}

void Balancer::EraseTable(std::string table) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto currentPartitions = GetPartition(table);
    for (auto [chunk, addBytes]: currentPartitions) {
        auto prevSize = chunk2Free_[chunk];
        freePositions_.erase({prevSize, chunk});
        auto freeBytes = addBytes + prevSize;
        chunk2Free_[chunk] = freeBytes;
        freePositions_.erase({freeBytes, chunk});
    }
    KVStorage_.erase(table);
}

std::vector<std::pair<Balancer::ChunkInfo, int>> Balancer::GetPartition(std::string table) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto elem: KVStorage_.values()) {
        std::cout << "balancer status " << elem.first << ' ' << elem.second << '\n';
    }
    return DeserializeValue(KVStorage_.values().at(table));
}
