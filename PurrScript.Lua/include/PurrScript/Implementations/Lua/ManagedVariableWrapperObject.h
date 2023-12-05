#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>

#include <memory>
#include <sol/sol.hpp>
#include <string_view>

#include "Helpers/TableHelpers.h"

namespace PurrScript::Implementations::Lua {

    // Forward declarations
    namespace Helpers::VariableTableHelpers {
        sol::object ToLuaObject(sol::this_state s, ScriptContext* scriptContext, Value* value);
    }

    class ManagedVariableWrapperObject {
        ScriptContext* _scriptContext;
        Value*         _value;
        Value*         _lhs = nullptr;

        inline sol::object index(sol::table table, std::string_view key, sol::this_state s) {
            return sol::nil;
        }

    public:
        ManagedVariableWrapperObject(ScriptContext* scriptContext, Value* value)
            : _scriptContext(scriptContext), _value(value) {
            if (value == nullptr) _Log_("[+] ManagedVariableWrapperObject (nullptr)");
            else _Log_("[+] ManagedVariableWrapperObject ({})", value->GetTypeName());
        }

        ~ManagedVariableWrapperObject() {
            _Log_("[~] ManagedVariableWrapperObject ({})", _value->GetTypeName());
        }

        ScriptContext* GetScriptContext() const { return _scriptContext; }
        Value*         GetValue() const { return _value; }

        void SetLHS(Value* lhs) {
            if (lhs == nullptr) _Log_("SetLHS(nullptr)");
            else _Log_("SetLHS({})", lhs->GetTypeName());
            _lhs = lhs;
        }
        Value* GetLHS() const { return _lhs; }

        sol::object LookupName(std::string_view nameToLookup, sol::this_state s) {
            _Log_("[lookup] {} -> {}", _value->GetTypeName(), nameToLookup);
            sol::state_view state(s);
            if (auto* foundVariable = _scriptContext->GetNameLookupHandlers()->LookupName(
                    nameToLookup.data(), _value, _scriptContext
                )) {
                return Helpers::VariableTableHelpers::ToLuaObject(s, _scriptContext, foundVariable);
            } else {
                _Log_("(Member not found '{}')", nameToLookup);
            }
            return sol::nil;
        }
    };
}
