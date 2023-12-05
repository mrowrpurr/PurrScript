#pragma once

#include "Core/CorePackage.h"  // IWYU pragma: keep

namespace PurrScript::Packages::Core {

    inline Package* GetPackage() { return CorePackage::Get().GetPackage(); }
}
