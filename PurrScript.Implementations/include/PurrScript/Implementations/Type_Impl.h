#pragma once

#include <PurrScript/API.h>

#include <string>
#include <string_view>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class Type_Impl : public Type, public Value_Impl {
        std::string _typeName = "";

    public:
        Type_Impl(std::string_view typeName, bool isDestructable = true)
            : Value_Impl(isDestructable), _typeName(typeName) {}

        const char* GetTypeName() override { return Type::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override { return new Type_Impl(_typeName, Value_Impl::_isDestuctable); }

        const char* GetName() override { return _typeName.c_str(); }
    };
}
