#pragma once

#include <PurrScript/API.h>

#include <memory>
#include <string_view>

#include "../Package_Impl.h"

namespace PurrScript::Implementations::Factories {

    inline Package_Impl CreatePackage(
        std::string_view name, std::string_view version = "", std::string_view author = ""
    ) {
        return Package_Impl(name, version, author);
    }
}
