#pragma once

#include <PurrScript/API.h>

#include "../StaticFunction_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline StaticFunction* CreateStaticFunction(
        std::unique_ptr<StaticFunctionFn> functionPointer, bool isDestructable = true
    ) {
        return new StaticFunction_Impl(std::move(functionPointer));
    }

    inline StaticFunction* CreateStaticFunction(
        StaticFunctionFn* functionPointer, bool isDestructable = true
    ) {
        return new StaticFunction_Impl(functionPointer);
    }
}
