#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <function_pointer.h>

#include <memory>
#include <string>
#include <string_view>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    // class Property_Impl : public Property, public Value_Impl {
    //     std::string                         _propertyName;
    //     std::string                         _propertyTypeName;
    //     InstanceFunctionFn*                 _unmanagedGetterFunction = nullptr;
    //     std::unique_ptr<InstanceFunctionFn> _managedGetterFunction   = nullptr;
    //     InstanceFunctionFn*                 _unmanagedSetterFunction = nullptr;
    //     std::unique_ptr<InstanceFunctionFn> _managedSetterFunction   = nullptr;

    // public:
    //     Property_Impl(
    //         std::string_view propertyName, std::string_view propertyTypeName,
    //         InstanceFunctionFn* getter, InstanceFunctionFn* setter, bool isDestructable = true
    //     )
    //         : Value_Impl(isDestructable),
    //           _propertyName(propertyName),
    //           _propertyTypeName(propertyTypeName),
    //           _unmanagedGetterFunction(getter),
    //           _unmanagedSetterFunction(setter) {}

    //     Property_Impl(
    //         std::string_view propertyName, std::string_view propertyTypeName,
    //         std::unique_ptr<InstanceFunctionFn> getter, std::unique_ptr<InstanceFunctionFn>
    //         setter, bool isDestructable = true
    //     )
    //         : Value_Impl(isDestructable),
    //           _propertyName(propertyName),
    //           _propertyTypeName(propertyTypeName),
    //           _managedGetterFunction(std::move(getter)),
    //           _managedSetterFunction(std::move(setter)) {}

    //     const char* GetTypeName() override { return Property::TYPE_NAME; }
    //     bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
    //     void        SetIsDestructable(bool isDestuctable = true) override {
    //         Value_Impl::SetIsDestructable(isDestuctable);
    //     }

    //     // Value* Copy() override {
    //     //     if (_managedFunction) {
    //     //         _Log_("Warning: Copying managed instance function. New copy will be
    //     unmanaged");
    //     //         return new Property_Impl(_managedFunction.get(), Value_Impl::_isDestuctable);
    //     //     }
    //     //     return new Property_Impl(_unmanagedFunction, Value_Impl::_isDestuctable);
    //     // }

    //     inline InstanceFunctionFn* GetGetter() const override {
    //         return _managedGetterFunction.get() ? _managedGetterFunction.get()
    //                                             : _unmanagedGetterFunction;
    //     }

    //     inline InstanceFunctionFn* GetSetter() const override {
    //         return _managedSetterFunction.get() ? _managedSetterFunction.get()
    //                                             : _unmanagedSetterFunction;
    //     }

    //     inline const char* GetName() const override { return _propertyName.data(); }

    //     inline const char* GetPropertyTypeName() const override { return
    //     _propertyTypeName.data(); }
    // };
}
