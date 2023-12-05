#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>

#include "Lua_LanguageContextManager_Impl.h"

namespace PurrScript::Implementations::Lua {

    class Lua_LanguageHandler_Impl : public LanguageHandler {
        PurrScriptAPI*                  _api;
        Lua_LanguageContextManager_Impl _contextManager{_api, this};

    public:
        Lua_LanguageHandler_Impl(PurrScriptAPI* api) : _api(api) {}

        ~Lua_LanguageHandler_Impl() override { _Log_("[~] Lua_LanguageHandler_Impl"); }

        LanguageContextManager* GetLanguageContextManager() override { return &_contextManager; }
    };
}
