#include <PurrScript/Client.h>
#include <PurrScript/DLL.h>
#include <PurrScript/Implementations/Factories/Void_Factory.h>

#include <iostream>
#include <memory>

// TODO: flatten namespaces and file paths plz :)

using namespace std;
using namespace PurrScript;
using namespace PurrScript::Client;
using namespace PurrScript::Implementations;

unique_ptr<PurrScriptAPI_Client> _client  = nullptr;
Package_Impl*                    _package = nullptr;

Value* say_hello_from_dll(ScriptContext*, FunctionArguments*) {
    cout << "Hello from global function defined in DLL!" << endl;
    return Factories::VoidValue();
}

PurrScript_DLL(PurrScript::PurrScriptAPI* api) {
    _client  = CreateClient(api);
    _package = _client->NewPackage("OneGlobalFunctionDLL", "1.0.0", "Mrowr").get();
    _package->DefineGlobalFunction(
        "say_hello_from_dll", unique_function_pointer(say_hello_from_dll)
    );
    api->GetPackageRegistry()->AddPackage(_package);
}
