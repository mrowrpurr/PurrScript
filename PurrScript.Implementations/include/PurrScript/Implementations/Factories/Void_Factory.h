#pragma once

#include <PurrScript/API.h>

#include "../Void_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline static Void* VoidValue(bool isDestructable = true) { return new Void_Impl(isDestructable); }
}
