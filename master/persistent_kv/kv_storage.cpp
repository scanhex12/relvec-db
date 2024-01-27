#include "kv_storage.h"

#include <fstream>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>


namespace NKV {

KVStorage::KVStorage(const char* filename) : filename_(filename), page_(FlixiblePage(filename)) {
    for (int i = 0; 2 * i + 1 < page_.getRowCount(); ++i) {
        std::string key = page_.getRow(2 * i);
        std::string value = page_.getRow(2 * i + 1);
        if (key.empty() || value.empty()) {
            continue;
        }
        values_[key] = value;
        key2index_[key] = i / 2;
    }
}

void KVStorage::set(std::string key, std::string value) {
    values_[key] = value;
    if (key2index_.count(key)) {
        erase(key);
    }
    keys_.push_back("");
    key2index_[key] = keys_.size() - 1;
    auto ret = page_.insertRow(key);
    assert(ret.first);
    ret = page_.insertRow(value);
    assert(ret.first);
}
 
void KVStorage::erase(std::string key) {
    if (!values_.count(key)) {
        return;
    }
    values_.erase(key);
    int index = key2index_[key];
    key2index_.erase(key);
    page_.deleteRow(2 * index);
    page_.deleteRow(2 * index + 1);
}

}
