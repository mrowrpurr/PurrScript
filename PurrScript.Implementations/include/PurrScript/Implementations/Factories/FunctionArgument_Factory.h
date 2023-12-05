#pragma once

#include <PurrScript/API.h>

#include <memory>
#include <string_view>

#include "../FunctionArgument_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline std::unique_ptr<FunctionArgument_Impl> CreateFunctionArgument(
        std::string_view name, std::uint8_t index, Value* value
    ) {
        return std::make_unique<FunctionArgument_Impl>(name, index, value);
    }

    inline std::unique_ptr<FunctionArgument_Impl> CreateFunctionArgument(
        std::uint8_t index, Value* value
    ) {
        return std::make_unique<FunctionArgument_Impl>(index, value);
    }
}
