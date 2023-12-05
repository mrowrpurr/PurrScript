#pragma once

#include <PurrScript/API.h>

#include <string>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class Float_Impl : public Float, public Value_Impl {
        double      _value = 0.0f;
        std::string _asString;

    public:
        Float_Impl(double value, bool isDestructable = true)
            : Value_Impl(isDestructable), _value(value) {}

        const char* GetTypeName() override { return Float::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override { return new Float_Impl(_value, Value_Impl::_isDestuctable); }

        double GetValue() override { return _value; }
        void   SetValue(double value) override { _value = value; }

        const char* AsString() override {
            _asString = std::to_string(_value);
            return _asString.c_str();
        }
    };
}
