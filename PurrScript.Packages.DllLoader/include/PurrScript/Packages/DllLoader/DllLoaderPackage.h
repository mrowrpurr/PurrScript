#pragma once

#include <PurrScript/API.h>
#include <PurrScript/DLL/DllLoader.h>
#include <PurrScript/Implementations/Factories/Boolean_Factory.h>
#include <PurrScript/Implementations/Factories/Package_Factory.h>

namespace PurrScript::Packages::DllLoader {

    class DllLoaderPackage {
        static constexpr auto PACKAGE_NAME    = "PurrScript.DllLoader";
        static constexpr auto PACKAGE_VERSION = "0.0.1";
        static constexpr auto PACKAGE_AUTHOR  = "Mrowr Purr";

        DLL::DllLoader _dllLoader;

        Implementations::Package_Impl _package = Implementations::Factories::CreatePackage(
            PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_AUTHOR
        );

        Value* LoadDLL(ScriptContext* context, FunctionArguments* args) {
            if (args->GetArgumentCount() != 1) {
                _Log_("LoadDLL requires 1 argument: DLL file path");
                return Implementations::Factories::VoidValue();
            }
            auto* dllFilePathValue = args->GetArgumentByIndex(0)->GetValue();
            if (!dllFilePathValue) {
                _Log_("LoadDLL requires 1 argument: DLL file path");
                return Implementations::Factories::VoidValue();
            }
            auto* dllFilePath = dllFilePathValue->AsString();
            try {
                _dllLoader.LoadDLL(context->GetAPI(), dllFilePath);
                _Log_("Loaded DLL: \"{}\"", dllFilePath);
                return Implementations::Factories::BooleanValue(true);
            } catch (const std::exception& e) {
                _Log_("Failed to load DLL: {}", e.what());
                return Implementations::Factories::BooleanValue(false);
            }
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
