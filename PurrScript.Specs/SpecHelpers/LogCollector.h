#pragma once

#include <string_format.h>

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

using namespace std;

static inline bool PrintLogs = false;  // <--- Set this to 'true' in your specs to see output

class LogCollector {
    stringstream _contents;

public:
    static LogCollector& Get() {
        static LogCollector instance;
        return instance;
    }

    void Append(string_view text) {
        if (PrintLogs) cout << text << "\n";
        _contents << text << "\n";
    }
    string GetContents() { return _contents.str(); }
    void   Clear() {
        _contents.str("");
        _contents.clear();
    }
    string ToString() {
        auto str = GetContents();
        Clear();
        return str;
    }
};
