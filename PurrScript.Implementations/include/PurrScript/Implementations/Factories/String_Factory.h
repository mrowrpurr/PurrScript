#pragma once

#include <PurrScript/API.h>

#include <string_view>

#include "../String_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline String_Impl* StringValue(std::string_view value, bool isDestructable = true) {
        return new String_Impl(value, isDestructable);
    }
}
