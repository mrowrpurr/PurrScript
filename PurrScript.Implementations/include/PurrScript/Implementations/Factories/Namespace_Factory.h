#pragma once

#include <PurrScript/API.h>

#include <string_view>

#include "../Namespace_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline Namespace_Impl* NamespaceValue(std::string_view value, bool isDestructable = true) {
        return new Namespace_Impl(value, isDestructable);
    }
}
