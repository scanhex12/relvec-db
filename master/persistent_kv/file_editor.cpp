#include "file_editor.h"


namespace NKV {

int FileEditor::getFileSize(std::string filename) {
    std::ifstream inputFile(filename, std::ios::binary);
    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.close();
    return fileSize;
}

void FileEditor::setFileSize(std::string filename, int size) {
    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
    file.seekp(size - 1, std::ios::beg);
    file.put('\0');
    file.close();
}

FileEditor::FileEditor(const std::string& filename, int cap) : filename_(filename) {
    getFileSize(filename);
    file_.open(filename, std::ios::in | std::ios::out);
    if (!file_.is_open()) {
        std::ofstream createFile(filename, std::ios::binary);
        createFile.close();
        file_.open(filename, std::ios::in | std::ios::out);
        if (!file_.is_open()) {
            throw std::runtime_error("Could not open the file.");
        }
    }
    if (getFileSize(filename) == 0) {
        setFileSize(filename, 8 * DISK_SIZE);   
    }
    assert(getFileSize(filename) == 8 * DISK_SIZE);
}

void FileEditor::Set(int i, char v) {
    if (file_.seekp(i)) {
        file_.put(v);
    } else {
        throw std::runtime_error("Failed to set the byte.");
    }
}

char FileEditor::Get(int i) {
    if (file_.seekg(i)) {
        return file_.get();
    } else {
        throw std::runtime_error("Failed to get the byte.");
    }
}

FileEditor::~FileEditor() {
    if (file_.is_open()) {
        file_.close();
    }
}

}
