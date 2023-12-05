#pragma once

#include <optional>
#include <sol/sol.hpp>
#include <string_view>

namespace PurrScript::Implementations::Lua::Helpers::TableHelpers {

    inline std::optional<sol::object> RawGetTableVariableFromStack(
        sol::this_state this_state, sol::table table, std::string_view variableName
    ) {
        lua_State* L = this_state;
        table.push(L);                           // Push the table onto the stack.
        lua_pushstring(L, variableName.data());  // Push the key onto the stack.

        // Retrieve the value directly from the table, bypassing the metatable.
        lua_rawget(L, -2);

        bool        exists = !lua_isnil(L, -1);
        sol::object value  = sol::stack::get<sol::object>(L, -1);

        lua_pop(L, 2);  // Clean the stack.

        if (exists) return value;
        else return std::nullopt;
    }
}
