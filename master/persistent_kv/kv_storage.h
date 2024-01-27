#pragma once

#include "flexible_page.h"

namespace NKV {

class KVStorage {
public:
    explicit KVStorage(const char* filename);

    void set(std::string key, std::string value);
    void erase(std::string key);

    const std::unordered_map<std::string, std::string>& values() const {
        return values_;
    }

private:
    std::vector<std::string> keys_;
    std::unordered_map<std::string, int> key2index_;
    std::unordered_map<std::string, std::string> values_;
    const char* filename_;

    FlixiblePage page_;
};

}