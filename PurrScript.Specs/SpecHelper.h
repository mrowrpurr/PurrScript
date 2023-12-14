#pragma once

#include "SpecHelpers/InterceptLogs.h"
// Include before everything else to override _Log_ to collect all logs from PurrScript (etc)

#include <PurrScript/Client.h>
#include <PurrScript/Implementations/Factories.h>
#include <PurrScript/Implementations/Lua/Lua_LanguageHandler_Impl.h>
#include <PurrScript/Implementations/PurrScriptAPI_Impl.h>
#include <PurrScript/Packages/Core.h>
// #include <Shared Example Struct Implementations.h>
#include <Specs.h>
#include <Specs/Snowhouse.h>
#include <string_format.h>

#include <iostream>  // IWYU pragma: keep

#include "SpecHelpers/LogCollector.h"

using namespace std;
using namespace PurrScript;
using namespace PurrScript::Implementations;
using namespace PurrScript::Implementations::Lua;
using namespace PurrScript::Implementations::Factories;

#define __PRINT_1(a) std::cout << string_format("{}", a) << std::endl
#define __PRINT_2(a, b) std::cout << string_format(a, b) << std::endl
#define __PRINT_3(a, b, c) std::cout << string_format(a, b, c) << std::endl
#define __PRINT_4(a, b, c, d) std::cout << string_format(a, b, c, d) << std::endl

#define __PRINT_EXPAND(x) x
#define __PRINT_GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define Print(...)                                                                              \
    __PRINT_EXPAND(                                                                             \
        __PRINT_GET_MACRO(__VA_ARGS__, __PRINT_4, __PRINT_3, __PRINT_2, __PRINT_1)(__VA_ARGS__) \
    )

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

inline string ReadLogs() { return LogCollector::Get().ToString(); }
