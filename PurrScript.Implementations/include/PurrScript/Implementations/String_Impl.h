#pragma once

#include <PurrScript/API.h>

#include <string>
#include <string_view>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class String_Impl : public String, public Value_Impl {
        std::string _value = "";

    public:
        String_Impl(std::string_view value, bool isDestructable = true)
            : Value_Impl(isDestructable), _value(value) {}

        const char* GetTypeName() override { return String::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override { return new String_Impl(_value, Value_Impl::_isDestuctable); }

        const char* GetValue() override { return _value.c_str(); }
        void        SetValue(const char* value) override { _value = value; }
    };
}
