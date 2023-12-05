#pragma once

#include <PurrScript/API.h>

#include "../StringMap_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline StringMap_Impl* StringValue(bool isDestructable = true) {
        return new StringMap_Impl(isDestructable);
    }
}
