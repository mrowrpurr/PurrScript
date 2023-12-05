#pragma once

#include <PurrScript/API.h>
#include <collections.h>

namespace PurrScript::Implementations {

    class PackageLookupHandlers_Impl : public PackageLookupHandlers {
        collections_set<PackageLookupHandlerFn*> _handlers;

    public:
        void AddHandler(PackageLookupHandlerFn* handler) override { _handlers.insert(handler); }

    private:
        FunctionPointer<void(PackageLookupHandlerFn* handler)> _addHandlerFn =
            function_pointer(this, &PackageLookupHandlers_Impl::AddHandler);

    public:
        Package* LookupImport(const char* requestedPackageName, ScriptContext* context) override {
            for (auto& handler : _handlers)
                if (auto* package = handler->invoke(requestedPackageName, context)) return package;
            return nullptr;
        }

        void ForEachHandler(ForEachHandlerFn* fn) override {
            for (auto& handler : _handlers) fn->invoke(handler);
        }

        void Merge(PackageLookupHandlers* other) override { other->ForEachHandler(&_addHandlerFn); }
    };
}
