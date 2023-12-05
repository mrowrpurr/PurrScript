#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <function_pointer.h>

#include <memory>

#include "Factories/Void_Factory.h"
#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class StaticFunction_Impl : public StaticFunction, public Value_Impl {
        StaticFunctionFn*                 _unmanagedFunction = nullptr;
        std::unique_ptr<StaticFunctionFn> _managedFunction   = nullptr;

    public:
        StaticFunction_Impl(StaticFunctionFn* function, bool isDestructable = true)
            : Value_Impl(isDestructable), _unmanagedFunction(function) {}

        StaticFunction_Impl(std::unique_ptr<StaticFunctionFn> function, bool isDestructable = true)
            : Value_Impl(isDestructable), _managedFunction(std::move(function)) {}

        const char* GetTypeName() override { return StaticFunction::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override {
            if (_managedFunction) {
                _Log_("Warning: Copying managed static function. New copy will be unmanaged");
                return new StaticFunction_Impl(_managedFunction.get(), Value_Impl::_isDestuctable);
            }
            return new StaticFunction_Impl(_unmanagedFunction, Value_Impl::_isDestuctable);
        }

        inline StaticFunctionFn* GetValue() override {
            return _managedFunction.get() ? _managedFunction.get() : _unmanagedFunction;
        }

        void SetValue(StaticFunctionFn* value) override { _unmanagedFunction = value; }
        void SetValue(std::unique_ptr<StaticFunctionFn> value) {
            _managedFunction = std::move(value);
        }

        Value* Call(ScriptContext* scriptContext, Value* typeValue, FunctionArguments* args)
            override {
            if (auto* type = typeValue->As<Type*>()) {
                return GetValue()->invoke(scriptContext, type, args);
            } else {
                _Log_(
                    "Warning: StaticFunction_Impl::Call() expected a Type* but got '{}'",
                    typeValue->GetTypeName()
                );
                return Factories::VoidValue();
            }
        }
    };
}
