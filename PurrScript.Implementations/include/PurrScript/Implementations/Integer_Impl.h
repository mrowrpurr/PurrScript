#pragma once

#include <PurrScript/API.h>

#include <string>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class Integer_Impl : public Integer, public Value_Impl {
        std::int64_t _value = 0;
        std::string  _asString;

    public:
        Integer_Impl(std::int64_t value, bool isDestructable = true)
            : Value_Impl(isDestructable), _value(value) {}

        const char* GetTypeName() override { return Integer::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override { return new Integer_Impl(_value, Value_Impl::_isDestuctable); }

        std::int64_t GetValue() override { return _value; }
        void         SetValue(std::int64_t value) override { _value = value; }

        const char* AsString() override {
            _asString = std::to_string(_value);
            return _asString.c_str();
        }
    };
}
