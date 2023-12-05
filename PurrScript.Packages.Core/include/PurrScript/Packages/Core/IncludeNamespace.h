//        using PackageLookupHandlerFn =
// IFunctionPointer<Package*(const char* requestedPackageName, ScriptContext* context)>;

#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/Factories/Boolean_Factory.h>
#include <_Log_.h>
#include <string_format.h>

#include <string>

namespace PurrScript::Packages::Core {

    class IncludeNamespace {
    public:
        static constexpr auto FUNCTION_NAME       = "include";
        static constexpr auto PACKAGE_NAME_KEY    = "from";
        static constexpr auto NAMESPACE_SEPARATOR = ".";

        // TODO: support table argument --> include("on", { from = "Events" })
        Value* include(ScriptContext* context, FunctionArguments* args) {
            bool anyNamespacesIncluded = false;

            auto argsCallback = unique_function_pointer([&](FunctionArgument* arg) {
                if (arg->GetValue()->IsMap()) return;

                std::string namespaceName{arg->GetValue()->AsString()};
                if (namespaceName.empty()) return;

                auto* variables = context->GetVariables();
                auto  start     = 0;
                auto  end       = namespaceName.find(NAMESPACE_SEPARATOR);

                while (end != std::string_view::npos) {
                    auto namespaceNamePart = namespaceName.substr(start, end - start);
                    _Log_(">> Namespace Part: {}", namespaceNamePart.data());
                    if (auto* variable =
                            context->GetVariables()->GetValue(namespaceNamePart.data())) {
                        if (variable->IsNamespace()) {
                            anyNamespacesIncluded = true;
                            variables             = variable->As<Namespace*>()->GetVariables();
                            // We don't want to add the variables of intermediate namespaces!
                            // context->GetVariables()->Merge(variable->As<Namespace*>()->GetVariables(
                            // ));
                        }
                    }
                    start = end + 1;
                    end   = namespaceName.find(NAMESPACE_SEPARATOR, start);
                }

                auto namespaceNamePart = namespaceName.substr(start, end - start);
                _Log_(">> (b) Namespace Part: {}", namespaceNamePart.data());
                if (auto* variable = variables->GetValue(namespaceNamePart.data())) {
                    if (variable->IsNamespace()) {
                        anyNamespacesIncluded = true;
                        context->GetVariables()->Merge(variable->As<Namespace*>()->GetVariables());
                    }
                }
            });

            // Is the last argument a { from = "Package Name" } ??
            if (args->GetArgumentByIndex(args->GetArgumentCount() - 1)->GetValue()->IsMap()) {
                auto* map =
                    args->GetArgumentByIndex(args->GetArgumentCount() - 1)->GetValue()->As<Map*>();
                if (auto* packageName = map->StringGetValue(PACKAGE_NAME_KEY)) {
                    if (packageName->IsString()) {
                        context->ImportPackage(packageName->AsString());
                    }
                }
            }

            args->ForEachArgument(argsCallback.get());

            return Implementations::Factories::BooleanValue(anyNamespacesIncluded);
        }

        FunctionPointer<Value*(ScriptContext*, FunctionArguments*)> IncludeFn =
            function_pointer(this, &IncludeNamespace::include);
    };
}
