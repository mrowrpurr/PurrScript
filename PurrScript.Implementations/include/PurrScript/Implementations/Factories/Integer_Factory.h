#pragma once

#include <PurrScript/API.h>

#include "../Integer_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline Integer_Impl* IntegerValue(std::int64_t value, bool isDestructable = true) {
        return new Integer_Impl(value);
    }
}
