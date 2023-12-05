#pragma once

#include <PurrScript/API.h>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class None_Impl : public None, public Value_Impl {
    public:
        None_Impl(bool isDestructable = true) : Value_Impl(isDestructable) {}

        const char* GetTypeName() override { return None::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override { return new None_Impl(Value_Impl::_isDestuctable); }
    };
}
