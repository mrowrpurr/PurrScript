#pragma once

#include <PurrScript/API.h>

#include "../Boolean_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline Boolean_Impl* BooleanValue(bool value, bool isDestructable = true) { return new Boolean_Impl(value); }
}
