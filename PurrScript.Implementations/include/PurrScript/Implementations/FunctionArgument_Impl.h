#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>

#include <string>
#include <string_view>

namespace PurrScript::Implementations {

    class FunctionArgument_Impl : public FunctionArgument {
        std::string  _name;
        std::uint8_t _index;
        Value*       _value;

    public:
        FunctionArgument_Impl(std::uint8_t index, Value* value) : _index(index), _value(value) {
            _Log_("[+] FunctionArgument_Impl");
        }
        FunctionArgument_Impl(std::string_view name, std::uint8_t index, Value* value)
            : _name(name), _index(index), _value(value) {
            _Log_("[+] FunctionArgument_Impl");
        }

        ~FunctionArgument_Impl() override {
            _Log_("[~] FunctionArgument_Impl");
            if (_value != nullptr) {
                if (_value->IsDestructable()) {
                    _Log_(
                        "Deleting destructable function argument value of type: {}",
                        _value->GetTypeName()
                    );
                    delete _value;
                }
            }
        }

        const char*  GetName() override { return _name.c_str(); }
        const char*  GetTypeName() override { return _value->GetTypeName(); }
        std::uint8_t GetIndex() override { return _index; }
        Value*       GetValue() override { return _value; }
    };
}
