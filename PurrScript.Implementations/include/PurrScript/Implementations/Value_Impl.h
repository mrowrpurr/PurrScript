#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>

namespace PurrScript::Implementations {

    class Value_Impl : public virtual Value {
    protected:
        bool _isDestuctable = true;

    public:
        Value_Impl(bool isDestuctable = true) : _isDestuctable(isDestuctable) {
            _Log_("[+] Value_Impl");
        }

        ~Value_Impl() override { _Log_("[~] Value_Impl"); }

        const char* GetTypeName() override { return Value::TYPE_NAME; }
        bool        IsDestructable() override { return _isDestuctable; }
        void        SetIsDestructable(bool isDestuctable = true) override {
            _isDestuctable = isDestuctable;
        }

        Value* Copy() override { return new Value_Impl(_isDestuctable); }
    };
}
