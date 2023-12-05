#pragma once

#include <PurrScript/API.h>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class Boolean_Impl : public Boolean, public Value_Impl {
        bool _value = false;

    public:
        Boolean_Impl(bool value, bool isDestructable = true)
            : Value_Impl(isDestructable), _value(value) {}

        const char* GetTypeName() override { return Boolean::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override { return new Boolean_Impl(_value, Value_Impl::_isDestuctable); }

        bool GetValue() override { return _value; }
        void SetValue(bool value) override { _value = value; }
    };
}
