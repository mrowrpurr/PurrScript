#pragma once

#include "Reflection/ReflectionPackage.h"  // IWYU pragma: keep

namespace PurrScript::Packages::Reflection {

    inline Package* GetPackage() { return ReflectionPackage::Get().GetPackage(); }
}
