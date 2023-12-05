#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/Factories.h>
#include <_Log_.h>

#include <memory>
#include <sol/sol.hpp>
#include <string>

#include "../ManagedVariableWrapperObject.h"
#include "TableGarbageCollectionHelpers.h"

namespace PurrScript::Implementations::Lua::Helpers::VariableTableHelpers {

    // TODO Move all this `inline` stuff into classes, please!

    inline const std::string OBJECT_IDENTIFY_AS_VALUE_KEY = "__PurrScript_ValueTable__";

    inline constexpr auto OBJECT_VARIABLE_WRAPPER_OBJECT_KEY = "var";
    inline constexpr auto OBJECT_GC_TRACKING_OBJECT_KEY      = "gc";
    inline constexpr auto OBJECT_SCRIPT_CONTEXT_KEY          = "ctx";

    sol::table CreateVariableTable(
        sol::state_view state, Value* value, ScriptContext* scriptContext
    );

    inline sol::object ToLuaObject(sol::this_state s, ScriptContext* scriptContext, Value* value) {
        if (value->IsNone() || value->IsVoid()) return sol::nil;
        sol::state_view state(s);
        if (value->IsBoolean()) return sol::make_object(state, value->AsBoolean());
        if (value->IsFloat()) return sol::make_object(state, value->AsFloat());
        if (value->IsInteger()) return sol::make_object(state, value->AsInteger());
        if (value->IsString()) return sol::make_object(state, value->AsString());
        return CreateVariableTable(s, value, scriptContext);
    }

    inline bool IsValueTable(sol::table table) {
        return table[OBJECT_IDENTIFY_AS_VALUE_KEY].valid();
    }

    inline Value* ToValue(sol::object luaObject, bool copy = true) {
        switch (luaObject.get_type()) {
            case sol::type::boolean:
                return Implementations::Factories::BooleanValue(luaObject.as<bool>());
            case sol::type::number: {
                auto asFloat       = luaObject.as<double>();
                auto isWholeNumber = std::floor(asFloat) == asFloat;
                if (isWholeNumber) return Implementations::Factories::IntegerValue(asFloat);
                else return Implementations::Factories::FloatValue(asFloat);
            }
            case sol::type::string:
                return Implementations::Factories::StringValue(luaObject.as<std::string>());
            default:
                if (luaObject.is<sol::table>()) {
                    auto table = luaObject.as<sol::table>();
                    if (IsValueTable(table)) {
                        auto variableWrapper = table[OBJECT_VARIABLE_WRAPPER_OBJECT_KEY]
                                                   .get<ManagedVariableWrapperObject*>();
                        if (copy) return variableWrapper->GetValue()->Copy();
                        else return variableWrapper->GetValue();
                    } else {
                        auto* map = Implementations::Factories::MapValue();
                        table.for_each([&map](sol::object key, sol::object value) {
                            switch (key.get_type()) {
                                case sol::type::number:
                                    map->IntegerSetValue(key.as<std::int64_t>(), ToValue(value));
                                    break;
                                case sol::type::string:
                                    map->StringSetValue(
                                        key.as<std::string>().c_str(), ToValue(value)
                                    );
                                    break;
                                default:
                                    _Log_("Unsupported key type in table");
                            }
                        });
                        return map;
                    }
                }
                return nullptr;
        }
        return nullptr;
    }

    inline std::unique_ptr<FunctionArguments> ToFunctionArguments(sol::variadic_args luaArgs) {
        auto functionArguments = Implementations::Factories::CreateFunctionArguments();
        for (auto arg : luaArgs) {
            if (auto* value = ToValue(arg, true)) functionArguments->AddArgument(value);
            else functionArguments->AddArgument(Implementations::Factories::NoneValue());
        }
        return std::move(functionArguments);
    }

    inline ManagedVariableWrapperObject* GetVariableWrapperObject(sol::table table) {
        return table[OBJECT_VARIABLE_WRAPPER_OBJECT_KEY].get<ManagedVariableWrapperObject*>();
    }

    inline sol::object index(sol::table table, std::string_view nameToLookup, sol::this_state s) {
        _Log_("Variable Table :: index({})", nameToLookup);

        if (nameToLookup == OBJECT_IDENTIFY_AS_VALUE_KEY) return sol::make_object(s, true);

        auto variableWrapper = GetVariableWrapperObject(table);

        auto object = variableWrapper->LookupName(nameToLookup, s);

        // When we invoke anything on a variable table, update the LHS of the variable wrapper
        if (object.is<sol::table>()) {
            sol::table objectTable = object;
            if (IsValueTable(objectTable))
                if (auto* lhsWrapper = GetVariableWrapperObject(objectTable))
                    lhsWrapper->SetLHS(variableWrapper->GetValue());
        }

        return object;
    }

    inline sol::object call(sol::table table, sol::variadic_args luaArgs, sol::this_state s) {
        auto variableWrapper =
            table[OBJECT_VARIABLE_WRAPPER_OBJECT_KEY].get<ManagedVariableWrapperObject*>();

        auto* scriptContext = table[OBJECT_SCRIPT_CONTEXT_KEY].get<ScriptContext*>();

        // Special support for Callable without looking up '()' function
        auto* value = variableWrapper->GetValue();

        if (value)
            _Log_(
                "Variable Table :: call() on type {}", variableWrapper->GetValue()->GetTypeName()
            );
        else _Log_("Variable Table :: call() on nullptr");

        if (value && value->IsCallable()) {
            _Log_("This type IsCallable()");
            // The value can be called directly
            auto  args = ToFunctionArguments(luaArgs);
            auto* self = variableWrapper->GetLHS();
            return ToLuaObject(s, scriptContext, value->Call(scriptContext, self, args.get()));
        }

        if (auto* function = scriptContext->GetNameLookupHandlers()->LookupName(
                SpecialFunctions::CALL, variableWrapper->GetValue(), scriptContext
            )) {
            _Log_("This type implements the custom CALL function");
            auto args = ToFunctionArguments(luaArgs);
            return ToLuaObject(s, scriptContext, function->Call(scriptContext, value, args.get()));
        } else {
            _Log_("[Error] '' is not a callable", value->GetTypeName());
            return sol::nil;
        }
    }

    inline sol::table CreateVariableTable(
        sol::state_view state, Value* value, ScriptContext* scriptContext
    ) {
        _Log_("CreateVariableTable value type: {}", value->GetTypeName());

        auto variableTable = state.create_table_with();

        // Metadata identifying this as a PurrScript value table (and providing the scriptContext)
        variableTable[OBJECT_IDENTIFY_AS_VALUE_KEY] = true;
        variableTable[OBJECT_SCRIPT_CONTEXT_KEY]    = scriptContext;

        // Setup index
        variableTable[sol::metatable_key]        = variableTable;
        variableTable[sol::meta_function::index] = index;
        variableTable[sol::meta_function::call]  = call;

        // Setup variable wrapper
        auto* variableWrapperObject = new ManagedVariableWrapperObject(scriptContext, value);
        variableTable[OBJECT_VARIABLE_WRAPPER_OBJECT_KEY] = variableWrapperObject;

        // Setup GC
        auto garbageCollectionDetectionObject =
            TableGarbageCollectionHelpers::CreateObjectWithGarbageCollectionCallback(
                state,
                [variableWrapperObject]() {
                    _Log_("Garbage collection callback called");
                    _Log_("Deleting the managed object which managed the variable and whatever");
                    delete variableWrapperObject;
                }
            );
        variableTable[OBJECT_GC_TRACKING_OBJECT_KEY] = garbageCollectionDetectionObject;

        return variableTable;
    }

    inline sol::table CreateVariableTable(
        sol::this_state s, Value* value, ScriptContext* scriptContext
    ) {
        return CreateVariableTable(sol::state_view(s), value, scriptContext);
    }
}
