#pragma once

#include "LibraryLoader/LibraryLoaderPackage.h"  // IWYU pragma: keep

namespace PurrScript::Packages::LibraryLoader {

    inline Package* GetPackage() { return LibraryLoaderPackage::Get().GetPackage(); }
}
