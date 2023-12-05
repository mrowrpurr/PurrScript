#pragma once

#include <PurrScript/API.h>

#include "../Function_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline Function* CreateFunction(
        std::unique_ptr<FunctionFn> functionPointer, bool isDestructable = true
    ) {
        return new Function_Impl(std::move(functionPointer));
    }

    inline Function* CreateFunction(FunctionFn* functionPointer, bool isDestructable = true) {
        return new Function_Impl(functionPointer);
    }
}
