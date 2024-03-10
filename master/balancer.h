#pragma once

#include <string>
#include "persistent_kv/kv_storage.h"
#include <set>
#include <map>
#include <mutex>

class Balancer {
public:
    using ChunkInfo = std::pair<std::string, int>;

    Balancer(const char* filename);

    void OnUpdate(ChunkInfo chunk, size_t freeBytes);

    void InsertTable(std::string table, size_t table_size);

    void EraseTable(std::string table);

    std::vector<std::pair<ChunkInfo, int>> GetPartition(std::string table);

private:
    std::string SerializeChunk(ChunkInfo info);
    ChunkInfo DeserializeChunk(std::string serialized);

    std::string SerializeValue(const std::vector<std::pair<ChunkInfo, int>>& data);
    std::vector<std::pair<ChunkInfo, int>> DeserializeValue(const std::string& str);

    NKV::KVStorage KVStorage_;
    std::set<std::pair<int, ChunkInfo>> freePositions_;
    std::map<ChunkInfo, int> chunk2Free_;
    std::mutex mutex_;
};
