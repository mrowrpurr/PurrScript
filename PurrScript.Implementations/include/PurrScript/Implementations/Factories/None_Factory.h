#pragma once

#include <PurrScript/API.h>

#include "../None_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline None_Impl* NoneValue(bool isDestructable = true) {
        return new None_Impl(isDestructable);
    }
}
