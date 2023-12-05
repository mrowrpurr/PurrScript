#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <function_pointer.h>

#include <memory>

#include "Factories/Void_Factory.h"
#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class Function_Impl : public Function, public Value_Impl {
        FunctionFn*                 _unmanagedFunction = nullptr;
        std::unique_ptr<FunctionFn> _managedFunction   = nullptr;

    public:
        Function_Impl(FunctionFn* function, bool isDestructable = true)
            : Value_Impl(isDestructable), _unmanagedFunction(function) {}

        Function_Impl(std::unique_ptr<FunctionFn> function, bool isDestructable = true)
            : Value_Impl(isDestructable), _managedFunction(std::move(function)) {}

        const char* GetTypeName() override { return Function::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override {
            if (_managedFunction) {
                _Log_("Warning: Copying managed function. New copy will be unmanaged");
                return new Function_Impl(_managedFunction.get(), Value_Impl::_isDestuctable);
            }
            return new Function_Impl(_unmanagedFunction, Value_Impl::_isDestuctable);
        }

        inline FunctionFn* GetValue() override {
            return _managedFunction.get() ? _managedFunction.get() : _unmanagedFunction;
        }

        void SetValue(FunctionFn* value) override { _unmanagedFunction = value; }
        void SetValue(std::unique_ptr<FunctionFn> value) { _managedFunction = std::move(value); }

        Value* Call(ScriptContext* scriptContext, Value*, FunctionArguments* args) override {
            return GetValue()->invoke(scriptContext, args);
        }
    };
}
