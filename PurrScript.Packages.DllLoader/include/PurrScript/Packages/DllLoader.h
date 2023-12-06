#pragma once

#include "DllLoader/DllLoaderPackage.h"  // IWYU pragma: keep

namespace PurrScript::Packages::DllLoader {

    inline Package* GetPackage() { return DllLoaderPackage::Get().GetPackage(); }
}
