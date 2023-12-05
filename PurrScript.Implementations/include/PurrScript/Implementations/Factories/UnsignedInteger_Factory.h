#pragma once

#include <PurrScript/API.h>

#include "../UnsignedInteger_Impl.h"

namespace PurrScript::Implementations {

    inline UnsignedInteger_Impl* UnsignedIntegerValue(
        std::uint64_t value, bool isDestructable = true
    ) {
        return new UnsignedInteger_Impl(value, isDestructable);
    }
}
