#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/Factories/Package_Factory.h>
#include <PurrScript/Implementations/Factories/Void_Factory.h>

#include <iostream>

namespace PurrScript::Packages::Console {

    class ConsolePackage {
        static constexpr auto PACKAGE_NAME    = "PurrScript.Console";
        static constexpr auto PACKAGE_VERSION = "0.0.1";
        static constexpr auto PACKAGE_AUTHOR  = "Mrowr Purr";

        Implementations::Package_Impl _package = Implementations::Factories::CreatePackage(
            PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_AUTHOR
        );

        void PrintFunction_ForEachArgument(FunctionArgument* argument) {
            if (argument)
                if (auto* value = argument->GetValue()) std::cout << value->AsString() << std::endl;
        }

        FunctionPointer<void(FunctionArgument*)> PrintFunction_ForEachArgumentFn =
            function_pointer(this, &ConsolePackage::PrintFunction_ForEachArgument);

        Value* PrintFunction(ScriptContext*, FunctionArguments* arguments) {
            arguments->ForEachArgument(&PrintFunction_ForEachArgumentFn);
            return Implementations::Factories::VoidValue();
        }

        FunctionPointer<Value*(ScriptContext*, FunctionArguments*)> PrintFunctionFn =
            function_pointer(this, &ConsolePackage::PrintFunction);

    public:
        ConsolePackage() { _package.DefineGlobalFunction("print", &PrintFunctionFn); }

        static ConsolePackage& Get() {
            static ConsolePackage instance;
            return instance;
        }

        Package* GetPackage() { return &_package; }
    };
}
