#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <function_pointer.h>

#include <memory>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class InstanceFunction_Impl : public InstanceFunction, public Value_Impl {
        InstanceFunctionFn*                 _unmanagedFunction = nullptr;
        std::unique_ptr<InstanceFunctionFn> _managedFunction   = nullptr;

    public:
        InstanceFunction_Impl(InstanceFunctionFn* function, bool isDestructable = true)
            : Value_Impl(isDestructable), _unmanagedFunction(function) {}

        InstanceFunction_Impl(
            std::unique_ptr<InstanceFunctionFn> function, bool isDestructable = true
        )
            : Value_Impl(isDestructable), _managedFunction(std::move(function)) {}

        const char* GetTypeName() override { return InstanceFunction::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override {
            if (_managedFunction) {
                _Log_("Warning: Copying managed instance function. New copy will be unmanaged");
                return new InstanceFunction_Impl(
                    _managedFunction.get(), Value_Impl::_isDestuctable
                );
            }
            return new InstanceFunction_Impl(_unmanagedFunction, Value_Impl::_isDestuctable);
        }

        inline InstanceFunctionFn* GetValue() override {
            return _managedFunction.get() ? _managedFunction.get() : _unmanagedFunction;
        }

        void SetValue(InstanceFunctionFn* value) override { _unmanagedFunction = value; }
        void SetValue(std::unique_ptr<InstanceFunctionFn> value) {
            _managedFunction = std::move(value);
        }

        Value* Call(ScriptContext* scriptContext, Value* self, FunctionArguments* args) override {
            return GetValue()->invoke(scriptContext, self, args);
        }
    };
}
