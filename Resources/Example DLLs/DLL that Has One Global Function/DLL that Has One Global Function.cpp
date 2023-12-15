#include <PurrScript/Client.h>
#include <PurrScript/DLL.h>
#include <PurrScript/Implementations/Factories/Void_Factory.h>

#include <iostream>

// TODO: flatten namespaces and file paths plz :)

using namespace std;
using namespace PurrScript;
using namespace PurrScript::Client;
using namespace PurrScript::Implementations;

Value* say_hello_from_dll(ScriptContext*, FunctionArguments*) {
    cout << "Hello from global function defined in DLL!" << endl;
    return Factories::VoidValue();
}

PurrScript_DLL(PurrScript::PurrScriptAPI* api) {
    auto  client  = CreateClient(api);
    auto& package = client->NewPackage("OneGlobalFunctionDLL", "1.0.0", "Mrowr");
    api->GetPackageRegistry()->AddPackage(package.get());
    package->DefineGlobalFunction(
        "say_hello_from_dll", unique_function_pointer(say_hello_from_dll)
    );
}
