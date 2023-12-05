#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/Factories/Package_Factory.h>

namespace PurrScript::Packages::Reflection {

    class ReflectionPackage {
        static constexpr auto PACKAGE_NAME    = "PurrScript.Reflection";
        static constexpr auto PACKAGE_VERSION = "0.0.1";
        static constexpr auto PACKAGE_AUTHOR  = "Mrowr Purr";

        Implementations::Package_Impl _package = Implementations::Factories::CreatePackage(
            PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_AUTHOR
        );

    public:
        ReflectionPackage() {
            // Example code:
            // print(vars())
            // print(vars(SomeNamespace))
        }

        static ReflectionPackage& Get() {
            static ReflectionPackage instance;
            return instance;
        }

        Package* GetPackage() { return &_package; }
    };
}
