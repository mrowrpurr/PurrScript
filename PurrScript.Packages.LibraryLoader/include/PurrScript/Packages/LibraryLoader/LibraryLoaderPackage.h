#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/Factories/Boolean_Factory.h>
#include <PurrScript/Implementations/Factories/Package_Factory.h>
#include <PurrScript/SharedLibraryLoader.h>

namespace PurrScript::Packages::LibraryLoader {

    class LibraryLoaderPackage {
        static constexpr auto PACKAGE_NAME    = "PurrScript.LibraryLoader";
        static constexpr auto PACKAGE_VERSION = "0.0.1";
        static constexpr auto PACKAGE_AUTHOR  = "Mrowr Purr";

        SharedLibraryLoader _libraryLoader;

        Implementations::Package_Impl _package = Implementations::Factories::CreatePackage(
            PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_AUTHOR
        );

        Value* LoadSharedLibrary(ScriptContext* context, FunctionArguments* args) {
            if (args->GetArgumentCount() != 1) {
                _Log_("LoadSharedLibrary requires 1 argument: DLL file path");
                return Implementations::Factories::VoidValue();
            }
            auto* dllFilePathValue = args->GetArgumentByIndex(0)->GetValue();
            if (!dllFilePathValue) {
                _Log_("LoadSharedLibrary requires 1 argument: DLL file path");
                return Implementations::Factories::VoidValue();
            }
            auto* dllFilePath = dllFilePathValue->AsString();
            try {
                _libraryLoader.LoadSharedLibrary(context->GetAPI(), dllFilePath);
                _Log_("Loaded DLL: \"{}\"", dllFilePath);
                return Implementations::Factories::BooleanValue(true);
            } catch (const std::exception& e) {
                _Log_("Failed to load DLL: {}", e.what());
                return Implementations::Factories::BooleanValue(false);
            }
        }

    public:
        LibraryLoaderPackage() {
            auto* LibraryLoaderNamespace = _package.DefineNamespace("LibraryLoader");

            _package.DefineGlobalFunction(
                "LoadSharedLibrary",
                unique_function_pointer(this, &LibraryLoaderPackage::LoadSharedLibrary),
                LibraryLoaderNamespace
            );
        }

        static LibraryLoaderPackage& Get() {
            static LibraryLoaderPackage instance;
            return instance;
        }

        Package* GetPackage() { return &_package; }
    };
}
