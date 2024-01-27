#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>


namespace NKV {

static constexpr size_t DISK_SIZE = 10000000;

class FileEditor {
    int getFileSize(std::string filename);

    void setFileSize(std::string filename, int size);
public:
    FileEditor(const std::string& filename, int cap);

    void Set(int i, char v);

    char Get(int i);

    ~FileEditor();

private:
    std::string filename_;
    std::fstream file_;
};

}
