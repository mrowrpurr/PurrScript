#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/Factories/Package_Factory.h>
#include <PurrScript/Implementations/Factories/Void_Factory.h>

namespace PurrScript::Packages::DllLoader {

    class DllLoaderPackage {
        static constexpr auto PACKAGE_NAME    = "PurrScript.DllLoader";
        static constexpr auto PACKAGE_VERSION = "0.0.1";
        static constexpr auto PACKAGE_AUTHOR  = "Mrowr Purr";

        Implementations::Package_Impl _package = Implementations::Factories::CreatePackage(
            PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_AUTHOR
        );

        Value* LoadDLL(ScriptContext*, FunctionArguments*) {
            _Log_("HELLO from _Log_ you called LoadDLL()!");
            return Implementations::Factories::VoidValue();
        }

    public:
        DllLoaderPackage() {
            auto* dllLoaderNamespace = _package.DefineNamespace("DllLoader");

            _package.DefineGlobalFunction(
                "LoadDLL", unique_function_pointer(this, &DllLoaderPackage::LoadDLL),
                dllLoaderNamespace
            );
        }

        static DllLoaderPackage& Get() {
            static DllLoaderPackage instance;
            return instance;
        }

        Package* GetPackage() { return &_package; }
    };
}
