#pragma once

#include <sol/sol.hpp>

#include "../TableGarbageCollectionCallback.h"

namespace PurrScript::Implementations::Lua::Helpers::TableGarbageCollectionHelpers {

    inline sol::object CreateObjectWithGarbageCollectionCallback(
        lua_State* L, std::function<void()> callback
    ) {
        // Create the user data with a destructor callback
        auto* data = static_cast<TableGarbageCollectionCallback*>(
            lua_newuserdata(L, sizeof(TableGarbageCollectionCallback))
        );
        new (data) TableGarbageCollectionCallback(callback);  // Placement new

        // Set the metatable for the userdata to give it the __gc method
        lua_createtable(L, 0, 1);  // Create a new table (metatable) on the stack
        lua_pushcfunction(L, [](lua_State* L) {
            auto* obj = static_cast<TableGarbageCollectionCallback*>(lua_touserdata(L, -1));
            obj->~TableGarbageCollectionCallback();  // Explicit destructor call
            return 0;
        });
        lua_setfield(L, -2, "__gc");

        lua_setmetatable(L, -2);  // Pops the metatable off the stack

        // Convert top of the Lua stack to a sol::object
        sol::stack_object stack_obj(L, -1);
        sol::object       user_data_obj = stack_obj;

        // Clean up the stack (optional, but can be important if you're doing a lot of stack
        // manipulations)
        lua_pop(L, 1);

        return user_data_obj;
    }

    inline sol::object CreateObjectWithGarbageCollectionCallback(
        sol::this_state s, std::function<void()> callback
    ) {
        return CreateObjectWithGarbageCollectionCallback(s.L, callback);
    }
}
