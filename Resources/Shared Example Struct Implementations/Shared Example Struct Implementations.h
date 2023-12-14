#pragma once

#include <_Log_.h>

#include <string>
#include <string_view>

#include "Shared Example Structs.h"

class Dog_Impl : public Dog {
    std::string _name;

public:
    Dog_Impl(std::string_view name) : _name(name) {
        std::uintptr_t address = (std::uintptr_t)this;
        _Log_("[+] Dog_Impl constructor called @ {:X}", address);
    }

    ~Dog_Impl() {
        std::uintptr_t address = (std::uintptr_t)this;
        _Log_("[~] Dog_Impl destructor called @ {:x}", address);
    }

    const char* GetName() override { return _name.c_str(); }
    void        Bark() override { _Log_("'Woof!' said {}", _name); }
};
