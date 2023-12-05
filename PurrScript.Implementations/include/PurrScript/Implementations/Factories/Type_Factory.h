#pragma once

#include <PurrScript/API.h>

#include <string_view>

#include "../Type_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline Type_Impl* TypeValue(std::string_view value, bool isDestructable = true) {
        return new Type_Impl(value, isDestructable);
    }
}
