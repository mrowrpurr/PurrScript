#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>

namespace PurrScript::Packages::Core {

    class ImportLookup {
    public:
        Package* LookupImport(const char* requestedPackageName, ScriptContext* context) {
            _Log_("LookupImport('{}')", requestedPackageName);

            // Check the global package registry
            if (auto* foundPackage =
                    context->GetAPI()->GetPackageRegistry()->GetPackage(requestedPackageName))
                return foundPackage;

            return nullptr;
        }

        FunctionPointer<Package*(const char*, ScriptContext*)> LookupImportFn =
            function_pointer(this, &ImportLookup::LookupImport);
    };
}
