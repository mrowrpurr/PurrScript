#pragma once

#include <PurrScript/Client.h>
#include <PurrScript/Implementations/Factories.h>
#include <PurrScript/Implementations/Lua/Lua_LanguageHandler_Impl.h>
#include <PurrScript/Implementations/PurrScriptAPI_Impl.h>
#include <PurrScript/Packages/Core.h>

#include "LogCollector.h"

using namespace std;
using namespace PurrScript;
using namespace PurrScript::Implementations;
using namespace PurrScript::Implementations::Lua;
using namespace PurrScript::Implementations::Factories;

class PurrScriptSpecEnvironment {
    Implementations::PurrScriptAPI_Impl       api;
    Client::PurrScriptAPI_Client              client{&api};
    unique_ptr<Implementations::Package_Impl> specHelpersPackage = nullptr;
    unique_ptr<LanguageHandler> luaLanguageHandler = make_unique<Lua_LanguageHandler_Impl>(&api);

    void PrintArgument(FunctionArgument* arg) {
        LogCollector::Get().Append(string_format("[print]{}[/print]", arg->GetValue()->AsString()));
    }

    FunctionPointer<void(FunctionArgument*)> _printArgumentFn =
        function_pointer(this, &PurrScriptSpecEnvironment::PrintArgument);

public:
    PurrScriptSpecEnvironment() {
        _Log_("[+] PurrScriptSpecEnvironment");

        api.SetDefaultLanguageHandler(luaLanguageHandler.get());
        api.GetPackageRegistry()->AddPackage(Packages::Core::GetPackage());
        api.GetDefaultPackages()->AddPackage(Packages::Core::GetPackage());

        specHelpersPackage = std::move(client.NewPackage("SpecHelpers"));
        specHelpersPackage->DefineGlobalFunction(
            "print",
            unique_function_pointer([&](ScriptContext*, FunctionArguments* args) -> Value* {
                args->ForEachArgument(&_printArgumentFn);
                return VoidValue();
            })
        );
    }

    ~PurrScriptSpecEnvironment() { _Log_("[~] PurrScriptSpecEnvironment"); }

    ScriptContext* CreateContext() {
        auto* context = api.GetScriptContextManager()->CreateContext();
        context->ImportPackage(specHelpersPackage.get());
        return context;
    }

    PurrScriptAPI* GetAPI() { return &api; }

    Client::PurrScriptAPI_Client* GetClient() { return &client; }

    std::unique_ptr<Implementations::Package_Impl>& SpecHelpersPackage() {
        return specHelpersPackage;
    }

    std::unique_ptr<Implementations::Package_Impl>& NewPackage(
        string_view packageName, bool addAsDefaultPackage = true
    ) {
        auto& package = client.NewPackage(packageName);
        if (addAsDefaultPackage) api.GetPackageRegistry()->AddPackage(package.get());
        return package;
    }
};