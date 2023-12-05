#pragma once

#include <PurrScript/API.h>

#include "None_Impl.h"

namespace PurrScript::Implementations {

    class Void_Impl : public Void, public None_Impl {
    public:
        Void_Impl(bool isDestructable = true) : None_Impl(isDestructable) {}

        const char*   GetTypeName() override { return Void::TYPE_NAME; }
        ValueType     GetValueType() override { return ValueType::Void; }
        bool          IsValueType() override { return true; }
        bool          IsNone() override { return true; }
        bool          AsBoolean() override { return false; }
        std::int64_t  AsInteger() override { return 0; }
        std::uint64_t AsUnsignedInteger() override { return 0; }
        double        AsFloat() override { return 0.0; }
        bool          IsDestructable() override { return None_Impl::IsDestructable(); }
        void          SetIsDestructable(bool isDestuctable = true) override {
            None_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override { return new Void_Impl(Value_Impl::_isDestuctable); }
    };
}
