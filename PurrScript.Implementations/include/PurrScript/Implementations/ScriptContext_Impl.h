#pragma once

// TODO: can this be moved into Implementations????

#include <PurrScript/API.h>
#include <_Log_.h>

#include "InstanceMemberRegistry_Impl.h"
#include "NameListHandlers_Impl.h"
#include "NameLookupHandlers_Impl.h"
#include "PackageCollection_Impl.h"
#include "PackageImportHandlers_Impl.h"
#include "PackageLookupHandlers_Impl.h"
#include "StaticMemberRegistry_Impl.h"
#include "TypeRegistry_Impl.h"
#include "VariableCollection_Impl.h"

namespace PurrScript::Implementations {

    class ScriptContext_Impl : public ScriptContext {
        PurrScriptAPI*              _api;
        LanguageHandler*            _languageHandler = nullptr;
        LanguageContext*            _languageContext = nullptr;
        VariableCollection_Impl     _variables;
        InstanceMemberRegistry_Impl _typeMemberRegistry;
        StaticMemberRegistry_Impl   _staticMemberRegistry;
        PackageCollection_Impl      _importedPackages;
        TypeRegistry_Impl           _typeRegistry;
        NameLookupHandlers_Impl     _nameLookupHandlers;
        NameListHandlers_Impl       _nameListHandlers;
        PackageLookupHandlers_Impl  _packageLookupHandlers;
        PackageImportHandlers_Impl  _packageImportHandlers;

        inline void ImportPackagePtr(Package* package) {
            if (_importedPackages.HasPackage(package)) {
                _Log_("Already imported package '{}' into this script context", package->GetName());
                return;
            }

            _Log_("ImportPackage('{}')", package->GetName());

            this->GetTypeRegistry()->Merge(package->GetTypeRegistry());
            this->GetVariables()->Merge(package->GetVariables());
            this->GetInstanceMemberRegistry()->Merge(package->GetInstanceMemberRegistry());
            this->GetStaticMemberRegistry()->Merge(package->GetStaticMemberRegistry());
            this->GetNameLookupHandlers()->Merge(package->GetNameLookupHandlers());
            this->GetNameListHandlers()->Merge(package->GetNameListHandlers());
            this->GetPackageLookupHandlers()->Merge(package->GetPackageLookupHandlers());
            this->GetPackageImportHandlers()->Merge(package->GetPackageImportHandlers());

            _importedPackages.AddPackage(package);

            // Report this package as imported (including to that package's included import
            // handlers)
            this->GetPackageImportHandlers()->RunHandlers(package, this);
        }

        FunctionPointer<void(Package*)> ImportPackagePtrFn =
            function_pointer(this, &ScriptContext_Impl::ImportPackagePtr);

    public:
        ScriptContext_Impl(PurrScriptAPI* api, LanguageHandler* languageHandler)
            : _api(api), _languageHandler(languageHandler) {
            _languageContext = _languageHandler->GetLanguageContextManager()->CreateContext(this);
        }

        ~ScriptContext_Impl() override { _Log_("[~] ScriptContext_Impl"); }

        PurrScriptAPI* GetAPI() override { return _api; }

        VariableCollection* GetVariables() override { return &_variables; }

        TypeRegistry*           GetTypeRegistry() override { return &_typeRegistry; }
        InstanceMemberRegistry* GetInstanceMemberRegistry() override {
            return &_typeMemberRegistry;
        }
        StaticMemberRegistry* GetStaticMemberRegistry() override { return &_staticMemberRegistry; }

        NameLookupHandlers* GetNameLookupHandlers() override { return &_nameLookupHandlers; }
        NameListHandlers*   GetNameListHandlers() override { return &_nameListHandlers; }

        PackageImportHandlers* GetPackageImportHandlers() override {
            return &_packageImportHandlers;
        }
        PackageLookupHandlers* GetPackageLookupHandlers() override {
            return &_packageLookupHandlers;
        }

        void ImportPackage(Package* package) override { ImportPackagePtr(package); }

        void ImportPackage(const char* requestedPackageName) override {
            if (auto* package = _packageLookupHandlers.LookupImport(requestedPackageName, this))
                ImportPackage(package);
        }

        void ImportPackages(ReadOnlyPackageCollection* collection) override {
            collection->ForEachPackage(&ImportPackagePtrFn);
        }

        ReadOnlyPackageCollection* GetImportedPackages() override { return nullptr; }

        ExecuteCodeResult* ExecuteCode(const char* code) override {
            return _languageContext->ExecuteCode(code);
        }

        ExecuteCodeResult* GetLastExecuteCodeResult() override { return nullptr; }

        LanguageHandler* GetLanguageHandler() override { return _languageHandler; }
    };
}
