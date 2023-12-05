#pragma once

#include <PurrScript/API.h>
#include <PurrScript/Implementations/ExecuteCodeResult_Impl.h>
#include <_Log_.h>

#include <sol/sol.hpp>
#include <string_view>

#include "GlobalContextTable.h"
#include "Helpers/VariableTableHelpers.h"

namespace PurrScript::Implementations::Lua {

    class Lua_LanguageContext_Impl : public LanguageContext {
        PurrScriptAPI*     _api;
        ScriptContext*     _scriptContext;
        sol::state         _luaState;
        GlobalContextTable _globalContextTable{_api, _scriptContext, _luaState};

    public:
        Lua_LanguageContext_Impl(PurrScriptAPI* api, ScriptContext* scriptContext)
            : _api(api), _scriptContext(scriptContext) {}

        ~Lua_LanguageContext_Impl() override { _Log_("[~] Lua_LanguageContext_Impl"); }

        ExecuteCodeResult* ExecuteCode(const char* code) override {
            _Log_("[Execute Code]{}[/Execute Code]", code);
            try {
                auto loadedScript = _luaState.load(code);
                if (loadedScript.valid()) {
                    _Log_("[Valid] Running...");
                    auto result = loadedScript();
                    if (result.valid()) {
                        _Log_("[Run OK]");
                        auto* value = Helpers::VariableTableHelpers::ToValue(result);
                        return ExecuteCodeResult_Impl::OK(value);
                    } else {
                        _Log_("[Run Failed] Result");
                        sol::error err = result;
                        _Log_("{}", err.what());
                        return ExecuteCodeResult_Impl::RuntimeError(err.what());
                    }
                } else {
                    _Log_("[Invalid]");
                    sol::error err = loadedScript;
                    _Log_("{}", err.what());
                    return ExecuteCodeResult_Impl::SyntaxError(nullptr, err.what());
                }
            } catch (const std::exception& e) {
                return ExecuteCodeResult_Impl::Exception(e.what());
            } catch (...) {
                return ExecuteCodeResult_Impl::UnknownException();
            }
        }
    };
}
