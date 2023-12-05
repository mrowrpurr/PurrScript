#pragma once

#include <PurrScript/API.h>
#include <collections.h>

namespace PurrScript::Implementations {

    class NameListHandlers_Impl : public NameListHandlers {
        collections_set<NameListFn*> _handlers;

    public:
        void AddHandler(NameListFn* handler) override { _handlers.insert(handler); }

    private:
        FunctionPointer<void(NameListFn* handler)> _addHandlerFn =
            function_pointer(this, &NameListHandlers_Impl::AddHandler);

    public:
        void ListNames(
            ScriptContext* parentScriptContext, Value* leftHandSideVariableIfAny,
            NameListCallbackFn* callback
        ) override {
            for (auto& handler : _handlers)
                handler->invoke(parentScriptContext, leftHandSideVariableIfAny, callback);
        }

        void ForEachHandler(ForEachFn* fn) override {
            for (auto& handler : _handlers) fn->invoke(handler);
        }

        void Merge(NameListHandlers* other) override { other->ForEachHandler(&_addHandlerFn); }
    };
}
