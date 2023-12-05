#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <collections.h>

#include <memory>
#include <span>
#include <string>
#include <string_view>

#include "Function_Impl.h"
#include "InstanceFunction_Impl.h"
#include "InstanceMemberRegistry_Impl.h"
#include "NameListHandlers_Impl.h"
#include "NameLookupHandlers_Impl.h"
#include "Namespace_Impl.h"
#include "PackageImportHandlers_Impl.h"
#include "PackageLookupHandlers_Impl.h"
#include "StaticFunction_Impl.h"
#include "StaticMemberRegistry_Impl.h"
#include "TypeRegistry_Impl.h"
#include "VariableCollection_Impl.h"

namespace PurrScript::Implementations {

    class Package_Impl : public Package {
        std::string _name;
        std::string _version;
        std::string _author;

        VariableCollection_Impl     _variables;
        InstanceMemberRegistry_Impl _instanceMemberRegistry;
        StaticMemberRegistry_Impl   _staticMemberRegistry;
        TypeRegistry_Impl           _typeRegistry;
        NameLookupHandlers_Impl     _nameLookupHandlers;
        NameListHandlers_Impl       _nameListHandlers;
        PackageImportHandlers_Impl  _packageImportHandlers;
        PackageLookupHandlers_Impl  _packageLookupHandlers;

        collections_set<std::unique_ptr<Function_Impl>>         _globalFunctions;
        collections_set<std::unique_ptr<InstanceFunction_Impl>> _instanceFunctions;
        collections_set<std::unique_ptr<StaticFunction_Impl>>   _staticFunctions;
        collections_set<std::unique_ptr<Namespace_Impl>>        _namespaces;

    public:
        Package_Impl(
            std::string_view name, std::string_view version = "", std::string_view author = ""
        )
            : _name(name), _version(version), _author(author) {
            _Log_("[+] Package_Impl");
        }

        ~Package_Impl() override { _Log_("[~] Package_Impl"); }

        const char* GetName() override { return _name.c_str(); }
        const char* GetVersion() override { return _version.c_str(); }
        const char* GetAuthor() override { return _author.c_str(); }

        VariableCollection* GetVariables() override { return &_variables; }

        TypeRegistry*           GetTypeRegistry() override { return &_typeRegistry; }
        InstanceMemberRegistry* GetInstanceMemberRegistry() override {
            return &_instanceMemberRegistry;
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

        Namespace_Impl* DefineNamespace(std::string_view name) {
            _Log_("Define Namespace '{}'", name);
            auto  theNamespace    = std::make_unique<Namespace_Impl>(name, false);
            auto* theNamespacePtr = theNamespace.get();
            _namespaces.emplace(std::move(theNamespace));
            _variables.AddVariable(name.data(), theNamespacePtr);
            return theNamespacePtr;
        }

        Namespace_Impl* DefineNamespaces(const std::vector<std::string_view>& names) {
            Namespace_Impl* theNamespace = nullptr;
            for (auto name : names) {
                _Log_("[loop] Define namespace {}", name);
                if (theNamespace == nullptr) theNamespace = DefineNamespace(name);
                else theNamespace = theNamespace->DefineChildNamespace(name);
            }
            return theNamespace;
        }

        void DefineInstanceMember(
            std::string_view typeName, std::string_view memberName,
            std::unique_ptr<InstanceFunctionFn> func
        ) {
            // Store the function which returns the member (given to use via param)
            auto theInstanceFunction =
                std::make_unique<Implementations::InstanceFunction_Impl>(std::move(func));
            auto* theInstanceFunctionPtr = theInstanceFunction.get();
            _instanceFunctions.emplace(std::move(theInstanceFunction));

            // Register the member
            _instanceMemberRegistry.AddMember(
                typeName.data(), memberName.data(), theInstanceFunctionPtr->GetValue()
            );
        }

        void DefineInstanceMember(
            std::string_view typeName, std::string_view memberName, InstanceFunctionFn* func
        ) {
            // Store the function which returns the member (given to use via param)
            auto theInstanceFunction =
                std::make_unique<Implementations::InstanceFunction_Impl>(func);
            auto* theInstanceFunctionPtr = theInstanceFunction.get();
            _instanceFunctions.emplace(std::move(theInstanceFunction));

            // Register the member
            _instanceMemberRegistry.AddMember(
                typeName.data(), memberName.data(), theInstanceFunctionPtr->GetValue()
            );
        }

        void DefineGlobalFunction(
            std::string_view functionName, std::unique_ptr<FunctionFn> func,
            Namespace* theNamespace = nullptr
        ) {
            // Store a unique 'Function' object here locally in the package
            auto theFunction =
                std::make_unique<Implementations::Function_Impl>(std::move(func), false);
            auto* theFunctionPtr = theFunction.get();
            _globalFunctions.emplace(std::move(theFunction));

            // Add it as a variable
            if (theNamespace)
                theNamespace->GetVariables()->AddVariable(functionName.data(), theFunctionPtr);
            else _variables.AddVariable(functionName.data(), theFunctionPtr);
        }

        void DefineGlobalFunction(
            std::string_view functionName, FunctionFn* func, Namespace* theNamespace = nullptr
        ) {
            // Store a unique 'Function' object here locally in the package
            auto  theFunction    = std::make_unique<Implementations::Function_Impl>(func, false);
            auto* theFunctionPtr = theFunction.get();
            _globalFunctions.emplace(std::move(theFunction));

            // Add it as a variable
            if (theNamespace)
                theNamespace->GetVariables()->AddVariable(functionName.data(), theFunctionPtr);
            else _variables.AddVariable(functionName.data(), theFunctionPtr);
        }

        void DefineInstanceFunction(
            std::string_view typeName, std::string_view memberName,
            std::unique_ptr<InstanceFunctionFn> func
        ) {
            // Store a unique 'InstanceFunction' object here locally in the package
            auto theInstanceFunction =
                std::make_unique<Implementations::InstanceFunction_Impl>(std::move(func));
            auto* theInstanceFunctionPtr = theInstanceFunction.get();
            _instanceFunctions.emplace(std::move(theInstanceFunction));

            // Register a getter for this 'InstanceFunction' object
            _instanceMemberRegistry.AddMember(
                typeName.data(), memberName.data(),
                unique_function_pointer(
                    [theInstanceFunctionPtr](
                        ScriptContext* scriptContext, Value* self, FunctionArguments* args
                    ) -> Value* { return theInstanceFunctionPtr; }
                )
            );
        }

        void DefineInstanceFunction(
            std::string_view typeName, std::string_view memberName, InstanceFunctionFn* func
        ) {
            // Store a unique 'InstanceFunction' object here locally in the package
            auto theInstanceFunction =
                std::make_unique<Implementations::InstanceFunction_Impl>(func);
            auto* theInstanceFunctionPtr = theInstanceFunction.get();
            _instanceFunctions.emplace(std::move(theInstanceFunction));

            // Register a getter for this 'InstanceFunction' object
            _instanceMemberRegistry.AddMember(
                typeName.data(), memberName.data(),
                unique_function_pointer(
                    [theInstanceFunctionPtr](
                        ScriptContext* scriptContext, Value* self, FunctionArguments* args
                    ) -> Value* { return theInstanceFunctionPtr; }
                )
            );
        }

        void DefineStaticFunction(
            std::string_view typeName, std::string_view memberName,
            std::unique_ptr<StaticFunctionFn> func
        ) {
            // Store a unique 'StaticFunction' object here locally in the package
            auto theStaticFunction =
                std::make_unique<Implementations::StaticFunction_Impl>(std::move(func));
            auto* theStaticFunctionPtr = theStaticFunction.get();
            _staticFunctions.emplace(std::move(theStaticFunction));

            // Register a getter for this 'StaticFunction' object
            _staticMemberRegistry.AddMember(
                typeName.data(), memberName.data(),
                unique_function_pointer(
                    [theStaticFunctionPtr](
                        ScriptContext* scriptContext, Type* type, FunctionArguments* args
                    ) -> Value* { return theStaticFunctionPtr; }
                )
            );
        }
    };
}
