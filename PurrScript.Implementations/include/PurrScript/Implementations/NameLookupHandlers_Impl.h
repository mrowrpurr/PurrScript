#pragma once

#include <PurrScript/API.h>
#include <collections.h>

namespace PurrScript::Implementations {

    class NameLookupHandlers_Impl : public NameLookupHandlers {
        collections_set<NameLookupHandlerFn*> _handlers;

    public:
        void AddHandler(NameLookupHandlerFn* handler) override { _handlers.insert(handler); }

    private:
        FunctionPointer<void(NameLookupHandlerFn* handler)> _addHandler =
            function_pointer(this, &NameLookupHandlers_Impl::AddHandler);

    public:
        Value* LookupName(
            const char* nameToLookup, Value* leftHandSideVariableValueIfAny,
            ScriptContext* parentScriptContext
        ) override {
            for (auto& handler : _handlers)
                if (auto* resultVariable = handler->invoke(
                        nameToLookup, leftHandSideVariableValueIfAny, parentScriptContext
                    ))
                    return resultVariable;
            return nullptr;
        }

        void ForEachHandler(ForEachNameHandlerFn* fn) override {
            for (auto& handler : _handlers) fn->invoke(handler);
        }

        void Merge(NameLookupHandlers* other) override { other->ForEachHandler(&_addHandler); }
    };
}
