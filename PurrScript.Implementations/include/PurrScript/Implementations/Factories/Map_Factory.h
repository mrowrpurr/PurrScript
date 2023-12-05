#pragma once

#include <PurrScript/API.h>

#include "../Map_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline Map_Impl* MapValue(bool isDestructable = true) { return new Map_Impl(isDestructable); }
}
