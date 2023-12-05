#pragma once

#include <PurrScript/API.h>
#include <void_pointer.h>

#include <string_view>

#include "../Object_Impl.h"

namespace PurrScript::Implementations::Factories {

    template <typename T>
    inline Object_Impl* CreateObject(
        std::string_view typeName, T* value, bool isDestructable = true
    ) {
        return new Object_Impl(typeName, new VoidPointer<T>(value), isDestructable);
    }

    inline Object_Impl* CreateObject(std::string_view typeName, bool isDestructable = true) {
        return new Object_Impl(typeName, isDestructable);
    }
}
