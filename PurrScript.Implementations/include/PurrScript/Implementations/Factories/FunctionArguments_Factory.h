#pragma once

#include <PurrScript/API.h>

#include <memory>

#include "../FunctionArguments_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline std::unique_ptr<FunctionArguments_Impl> CreateFunctionArguments() {
        return std::make_unique<FunctionArguments_Impl>();
    }
}
