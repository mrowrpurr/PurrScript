#pragma once

#include "Console/ConsolePackage.h"  // IWYU pragma: keep

namespace PurrScript::Packages::Console {

    inline Package* GetPackage() { return ConsolePackage::Get().GetPackage(); }
}
