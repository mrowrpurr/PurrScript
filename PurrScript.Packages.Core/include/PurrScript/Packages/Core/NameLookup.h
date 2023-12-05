#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/Factories/FunctionArguments_Factory.h>
#include <PurrScript/Implementations/Factories/String_Factory.h>
#include <PurrScript/Implementations/Factories/Type_Factory.h>
#include <_Log_.h>

namespace PurrScript::Packages::Core {

    class NameLookup {
    public:
        // TODO: consider splitting this up to make it composable - consider performance costs both
        // ways, though.
        // TODO: or consider moving lots of this to operators, but also: performance?
        Value* LookupName(
            const char* nameToLookup, Value* leftHandSideVariableValueIfAny, ScriptContext* context
        ) {
            _Log_("Core:LookupName '{}'", nameToLookup);

            // If there is a left hand side, check its type and look for members of that type:
            if (leftHandSideVariableValueIfAny) {
                if (leftHandSideVariableValueIfAny->IsType()) {
                    if (auto* type = leftHandSideVariableValueIfAny->As<Type*>()) {
                        if (auto* foundStaticMember =
                                context->GetStaticMemberRegistry()->GetMemberFunction(
                                    type->GetName(), nameToLookup
                                ))
                            return foundStaticMember->invoke(context, type, nullptr);
                    } else {
                        _Log_("[Core Name Lookup] left hand side is not a type ???");
                        return nullptr;
                    }
                } else {
                    if (auto* foundMember = context->GetInstanceMemberRegistry()->GetMemberFunction(
                            leftHandSideVariableValueIfAny->GetTypeName(), nameToLookup
                        )) {
                        return foundMember->invoke(
                            context, leftHandSideVariableValueIfAny, nullptr
                        );
                    }
                }
            }

            // Just look in the script context's variables - that's the default behavior
            if (auto* variable = context->GetVariables()->GetValue(nameToLookup)) return variable;

            // Next, check script context types
            if (context->GetTypeRegistry()->IsType(nameToLookup))
                return Implementations::Factories::TypeValue(nameToLookup);

            // Namespace check now...
            if (leftHandSideVariableValueIfAny && leftHandSideVariableValueIfAny->IsNamespace()) {
                // Find a matching variable
                if (auto* variable =
                        leftHandSideVariableValueIfAny->As<Namespace*>()->GetVariables()->GetValue(
                            nameToLookup
                        ))
                    return variable;
            }

            // No? Ok. Maybe the type itself implements the name lookup [] operator?
            if (leftHandSideVariableValueIfAny) {
                auto* typeName = leftHandSideVariableValueIfAny->GetTypeName();
                if (auto* foundCustomIndexFunction =
                        context->GetInstanceMemberRegistry()->GetMemberFunction(
                            typeName, SpecialFunctions::GETTER
                        )) {
                    auto args = Implementations::Factories::CreateFunctionArguments();
                    args->AddArgument(Implementations::Factories::StringValue(nameToLookup));
                    _Log_("Invoking custom index function on type {}", typeName);
                    return foundCustomIndexFunction->invoke(
                        context, leftHandSideVariableValueIfAny, args.get()
                    );
                }
            }

            return nullptr;
        }

        FunctionPointer<Value*(const char*, Value*, ScriptContext*)> LookupHandlerFn = {
            this, &NameLookup::LookupName
        };
    };
}
