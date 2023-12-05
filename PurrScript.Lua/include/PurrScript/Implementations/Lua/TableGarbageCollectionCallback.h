
#pragma once

#include <functional>

namespace PurrScript::Implementations::Lua {

    class TableGarbageCollectionCallback {
        std::function<void()> callback;

    public:
        TableGarbageCollectionCallback(std::function<void()> callback) : callback(callback) {}

        ~TableGarbageCollectionCallback() { callback(); }
    };
}
