#pragma once

#include <PurrScript/API.h>

#include "PackageCollection_Impl.h"
#include "ScriptContextManager_Impl.h"

namespace PurrScript::Implementations {

    class PurrScriptAPI_Impl : public PurrScriptAPI {
        LanguageHandler*          _defaultLanguageHandler = nullptr;
        ScriptContextManager_Impl _contextManager{this};
        PackageCollection_Impl    _defaultPackages;
        PackageCollection_Impl    _registeredPackages;

    public:
        ScriptContextManager* GetScriptContextManager() override { return &_contextManager; }

        void SetDefaultLanguageHandler(LanguageHandler* languageHandler) override {
            _defaultLanguageHandler = languageHandler;
        }

        LanguageHandler* GetDefaultLanguageHandler() override { return _defaultLanguageHandler; }

        PackageCollection* GetDefaultPackages() override { return &_defaultPackages; }

        PackageCollection* GetPackageRegistry() override { return &_registeredPackages; }
    };
}
