#pragma once

#include "file_editor.h"


namespace NKV {
    
class FlixiblePage {
    FileEditor fileEditor_;
    const int page_size = DISK_SIZE;
        
    int getByIndex(int index);

    void setByIndex(int index, int value);

    int getCntRows();

    std::vector<int> getSizesRows();

    void eraseLength(int index);

    void eraseRow(int len, int prevSum, int allSum);

public:
    FlixiblePage(const char* name);

    int getRowCount();

    std::string getRow(int index);

    void deleteRow(int index);

    std::pair<bool, int> insertRow(const std::string &row);
};

}
