#pragma once

#include <PurrScript/API.h>

#include "../Property_Impl.h"

namespace PurrScript::Implementations::Factories {

    // inline Property* CreateProperty(
    //     std::string_view propertyName, std::string_view propertyTypeName,
    //     std::unique_ptr<InstanceFunctionFn> getter, std::unique_ptr<InstanceFunctionFn> setter,
    //     bool isDestructable = true
    // ) {
    //     return new Property_Impl(
    //         propertyName, propertyTypeName, std::move(getter), std::move(setter), isDestructable
    //     );
    // }

    // inline Property* CreateProperty(
    //     std::string_view propertyName, std::string_view propertyTypeName,
    //     InstanceFunctionFn* getter, InstanceFunctionFn* setter, bool isDestructable = true
    // ) {
    //     return new Property_Impl(propertyName, propertyTypeName, getter, setter, isDestructable);
    // }
}
