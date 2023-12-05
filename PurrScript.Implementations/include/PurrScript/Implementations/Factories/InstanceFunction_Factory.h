#pragma once

#include <PurrScript/API.h>

#include <memory>

#include "../InstanceFunction_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline InstanceFunction* CreateInstanceFunction(
        std::unique_ptr<InstanceFunctionFn> functionPointer, bool isDestructable = true
    ) {
        return new InstanceFunction_Impl(std::move(functionPointer));
    }

    inline InstanceFunction* CreateInstanceFunction(
        InstanceFunctionFn* functionPointer, bool isDestructable = true
    ) {
        return new InstanceFunction_Impl(functionPointer);
    }
}
