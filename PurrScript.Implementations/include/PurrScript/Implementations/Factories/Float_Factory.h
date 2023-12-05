#pragma once

#include <PurrScript/API.h>

#include "../Float_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline Float_Impl* FloatValue(double value, bool isDestructable = true) { return new Float_Impl(value); }
}
