#pragma once

#include <PurrScript/API.h>
#include <collections.h>

#include <memory>

#include "Lua_LanguageContext_Impl.h"

namespace PurrScript::Implementations::Lua {

    class Lua_LanguageContextManager_Impl : public LanguageContextManager {
        PurrScriptAPI*   _api;
        LanguageHandler* _languageHandler = nullptr;
        collections_map<LanguageContext*, std::unique_ptr<Lua_LanguageContext_Impl>> _contexts;

    public:
        Lua_LanguageContextManager_Impl(PurrScriptAPI* api, LanguageHandler* languageHandler)
            : _api(api), _languageHandler(languageHandler) {}

        LanguageContext* CreateContext(ScriptContext* scriptContext) override {
            auto  context    = std::make_unique<Lua_LanguageContext_Impl>(_api, scriptContext);
            auto* contextPtr = context.get();
            _contexts.emplace(contextPtr, std::move(context));
            return contextPtr;
        }

        bool DestroyContext(LanguageContext* context) override {
            auto it = _contexts.find(context);
            if (it == _contexts.end()) return false;
            _contexts.erase(it);
            return true;
        }
    };
}
