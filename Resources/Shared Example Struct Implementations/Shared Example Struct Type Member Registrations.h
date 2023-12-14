#pragma once

#include <PurrScript/Client.h>
#include <PurrScript/Implementations/Factories/Void_Factory.h>
#include <_Log_.h>

#include "Shared Example Struct Implementations.h"

using namespace PurrScript;
using namespace PurrScript::Implementations;

void SetupDog(Client::PurrScriptAPI_Client* purrScript) {
    auto& package = purrScript->NewPackage("Dog");
    // package->GetInstanceMemberRegistry()->AddMember("Dog", "Name=", InstanceFunctionFn
    // *invocationFn) package->GetInstanceMemberRegistry()->AddMember("Dog", "Name",
    // InstanceFunctionFn *invocationFn)
    // package->GetInstanceMemberRegistry()->AddMember("Dog", "Bark", );
    package->DefineInstanceMember(
        "Dog", "Bark",
        unique_function_pointer(
            [](ScriptContext* scriptContext, Value* self, FunctionArguments* args) -> Value* {
                _Log_("Called the NEW Bark() defined function!");
                return Implementations::Factories::VoidValue();
            }
        )
    );
}

void SetupTypes(Client::PurrScriptAPI_Client* purrScript) { SetupDog(purrScript); }
