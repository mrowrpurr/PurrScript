#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/Factories/Package_Factory.h>
#include <_Log_.h>

#include "ImportLookup.h"
#include "IncludeNamespace.h"
#include "NameLookup.h"

namespace PurrScript::Packages::Core {

    class CorePackage {
        static constexpr auto PACKAGE_NAME    = "PurrScript.Core";
        static constexpr auto PACKAGE_VERSION = "0.0.1";
        static constexpr auto PACKAGE_AUTHOR  = "Mrowr Purr";

        Implementations::Package_Impl _package = Implementations::Factories::CreatePackage(
            PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_AUTHOR
        );

        NameLookup       _nameLookup;
        ImportLookup     _importLookup;
        IncludeNamespace _includeNamespace;

    public:
        CorePackage() {
            _package.GetNameLookupHandlers()->AddHandler(&_nameLookup.LookupHandlerFn);
            _package.GetPackageLookupHandlers()->AddHandler(&_importLookup.LookupImportFn);
            _package.DefineGlobalFunction(
                IncludeNamespace::FUNCTION_NAME, &_includeNamespace.IncludeFn
            );
        }

        static CorePackage& Get() {
            static CorePackage instance;
            return instance;
        }

        Package* GetPackage() { return &_package; }
    };
}
