#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>

#include <sol/sol.hpp>
#include <string_view>

#include "Helpers/TableHelpers.h"
#include "Helpers/VariableTableHelpers.h"

namespace PurrScript::Implementations::Lua {

    class GlobalContextTable {
        PurrScriptAPI* _api;
        ScriptContext* _scriptContext;

        static constexpr auto LUA_GLOBALS_TABLE_NAME = "_G";

        inline sol::object index(sol::table table, std::string_view key, sol::this_state s) {
            _Log_("GlobalContextTable.index({})", key);
            auto value = Helpers::TableHelpers::RawGetTableVariableFromStack(s, table, key);
            if (value) {
                _Log_("Returning value from the global table");
                return *value;
            }

            if (auto* foundVariable = _scriptContext->GetNameLookupHandlers()->LookupName(
                    key.data(), nullptr, _scriptContext
                )) {
                _Log_(
                    "GLOBAL Table: found variable {} type: {}", key, foundVariable->GetTypeName()
                );
                return Helpers::VariableTableHelpers::CreateVariableTable(
                    s, foundVariable, _scriptContext
                );
            }

            _Log_("GLOBAL Table: variable not found: {} returning NIL", key);
            return sol::nil;
        }

        inline void importFunction(std::string_view requestedPackageName) {
            if (requestedPackageName.empty()) return;
            _scriptContext->ImportPackage(requestedPackageName.data());
        }

    public:
        GlobalContextTable(PurrScriptAPI* api, ScriptContext* scriptContext, sol::state& state)
            : _api(api), _scriptContext(scriptContext) {
            // For set_function to work, we need to add the base library (unsure why...)
            state.open_libraries(sol::lib::base);

            state["print"] = sol::nil;  // Don't use the built-in print, we provide our own

            // The import() function is the only core function that is always provided
            state.set_function("import", &GlobalContextTable::importFunction, this);

            // Setup the Lua global state so any variable lookup calls our own __index
            sol::table globalContextTable = state[LUA_GLOBALS_TABLE_NAME];
            globalContextTable.set_function("__index", &GlobalContextTable::index, this);
            globalContextTable[sol::metatable_key] = globalContextTable;

            _Log_("[+] GlobalContextTable");
        }

        ~GlobalContextTable() { _Log_("[~] GlobalContextTable"); }
    };
}
