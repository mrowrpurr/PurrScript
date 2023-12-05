#pragma once

#include <PurrScript/API.h>
#include <collections.h>

namespace PurrScript::Implementations {

    class PackageImportHandlers_Impl : public PackageImportHandlers {
        collections_set<PackageImportHandlerFn*> _handlers;

    public:
        void AddHandler(PackageImportHandlerFn* handler) override { _handlers.insert(handler); }

    private:
        FunctionPointer<void(PackageImportHandlerFn* handler)> _addHandlerFn =
            function_pointer(this, &PackageImportHandlers_Impl::AddHandler);

    public:
        bool RunHandlers(Package* package, ScriptContext* context) override {
            for (auto& handler : _handlers) handler->invoke(package, context);
            return true;
        }

        void ForEachHandler(ForEachHandlerFn* fn) override {
            for (auto& handler : _handlers) fn->invoke(handler);
        }

        void Merge(PackageImportHandlers* other) override { other->ForEachHandler(&_addHandlerFn); }
    };
}
