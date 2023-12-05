#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <collections.h>

#include <memory>

#include "ScriptContext_Impl.h"

namespace PurrScript::Implementations {

    class ScriptContextManager_Impl : public ScriptContextManager {
        PurrScriptAPI*                                                       _api;
        collections_map<ScriptContext*, std::unique_ptr<ScriptContext_Impl>> _contexts;

    public:
        ScriptContextManager_Impl(PurrScriptAPI* api) : _api(api) {}

        ScriptContext* CreateContext(bool useDefaultPackages = true) override {
            auto* languageHandler = _api->GetDefaultLanguageHandler();
            if (!languageHandler) {
                _Log_("Failed To Create Context: Language Handler Is Null");
                return nullptr;
            }
            auto  context    = std::make_unique<ScriptContext_Impl>(_api, languageHandler);
            auto* contextPtr = context.get();
            _contexts.emplace(contextPtr, std::move(context));
            if (useDefaultPackages) contextPtr->ImportPackages(_api->GetDefaultPackages());
            return contextPtr;
        }

        ScriptContext* CreateContext(
            LanguageHandler* languageHandler, bool useDefaultPackages = true
        ) override {
            if (!languageHandler) {
                _Log_("Failed To Create Context: Language Handler Is Null");
                return nullptr;
            }
            auto  context    = std::make_unique<ScriptContext_Impl>(_api, languageHandler);
            auto* contextPtr = context.get();
            _contexts.emplace(contextPtr, std::move(context));
            if (useDefaultPackages) contextPtr->ImportPackages(_api->GetDefaultPackages());
            return contextPtr;
        }

        bool DestroyContext(ScriptContext* context) override {
            auto it = _contexts.find(context);
            if (it == _contexts.end()) return false;
            _contexts.erase(it);
            return true;
        }

        std::uint32_t GetContextCount() override { return _contexts.size(); }

        bool IsContextValid(ScriptContext* context) override {
            return _contexts.find(context) != _contexts.end();
        }

        void ForEachContext(ForEachContextFn* fn) override {
            for (auto& [context, _] : _contexts) fn->invoke(context);
        }
    };
}
