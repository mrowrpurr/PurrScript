#pragma once

#include <PurrScript/API.h>
#include <collections.h>

#include <string>

#include "Value_Impl.h"

namespace PurrScript::Implementations {

    class StringMap_Impl : public StringMap, public Value_Impl {
        collections_map<std::string, Value*> _map;

    public:
        StringMap_Impl(bool isDestructable = true) : Value_Impl(isDestructable) {}

        const char* GetTypeName() override { return StringMap::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override { return new StringMap_Impl(Value_Impl::_isDestuctable); }

        Value* GetValue(const char* key) override {
            auto found = _map.find(key);
            if (found == _map.end()) return nullptr;
            return found->second;
        }

        void SetValue(const char* key, Value* value) override { _map[key] = value; }

        bool RemoveValue(const char* key) override {
            auto found = _map.find(key);
            if (found == _map.end()) return false;
            _map.erase(found);
            return true;
        }

        std::uint64_t GetCount() override { return _map.size(); }

        void ForEachValue(ForEachValueFn* fn) override {
            for (auto& [key, value] : _map) fn->invoke(key.c_str(), value);
        }
    };
}
