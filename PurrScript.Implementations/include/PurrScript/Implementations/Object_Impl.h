#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <void_pointer.h>

#include <string>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class Object_Impl : public virtual Object, public Value_Impl {
    protected:
        std::string   _typeName       = Object::TYPE_NAME;
        IVoidPointer* _managedValue   = nullptr;
        void*         _unmanagedValue = nullptr;

    public:
        Object_Impl(std::string_view typeName, bool isDestructable = true)
            : Value_Impl(isDestructable), _typeName(typeName) {
            _Log_("[+] Object_Impl ({})", _typeName);
        }

        Object_Impl(
            std::string_view typeName, IVoidPointer* managedValue, bool isDestructable = true
        )
            : Value_Impl(isDestructable), _typeName(typeName), _managedValue(managedValue) {
            _Log_("[+] Object_Impl ({})", _typeName);
        }

        Object_Impl(std::string_view typeName, void* unmanagedValue, bool isDestructable = true)
            : Value_Impl(isDestructable), _typeName(typeName), _unmanagedValue(unmanagedValue) {
            _Log_("[+] Object_Impl ({})", _typeName);
        }

        ~Object_Impl() override {
            _Log_("[~] Object_Impl ({})", _typeName);
            if (_managedValue) delete _managedValue;
        }

        const char* GetTypeName() override { return _typeName.c_str(); }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override {
            _Log_("[COPY] Object_Impl ({})", _typeName);
            if (_managedValue != nullptr) {
                _Log_("Warning: Copying managed object. New copy will be unmanaged (void*)");
                return new Object_Impl(
                    _typeName, (void*)_managedValue->void_ptr(), Value_Impl::_isDestuctable
                );
            }
            return new Object_Impl(_typeName, _unmanagedValue, Value_Impl::_isDestuctable);
        }

        void* GetValue() override {
            if (_managedValue != nullptr) return _managedValue->void_ptr();
            return _unmanagedValue;
        }

        void CastTo(const char* typeName) override { _typeName = typeName; }
    };
}
