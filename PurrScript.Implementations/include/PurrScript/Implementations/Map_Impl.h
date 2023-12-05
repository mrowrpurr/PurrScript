#pragma once

#include <PurrScript/API.h>
#include <collections.h>

#include "Factories/None_Factory.h"
#include "Value_Impl.h"

namespace PurrScript::Implementations {

    // This is really basically a Lua table (allows int and string keys)
    // TODO: consider refactor / how to organize Maps for different key types
    // (and other collections)
    class Map_Impl : public Map, public Value_Impl {
        collections_map<std::string, Value*>  _stringMap;
        collections_map<std::int64_t, Value*> _intMap;

    public:
        Map_Impl(bool isDestructable = true) : Value_Impl(isDestructable) {}

        const char* GetTypeName() override { return Map::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        Value* Copy() override { return new Map_Impl(Value_Impl::_isDestuctable); }

        std::uint64_t GetCount() override { return _stringMap.size() + _intMap.size(); }

        Value* StringGetValue(const char* key) override {
            auto found = _stringMap.find(key);
            if (found == _stringMap.end()) return Factories::NoneValue();
            return found->second;
        }

        void StringSetValue(const char* key, Value* value) override { _stringMap[key] = value; }

        bool StringRemoveValue(const char* key) override {
            auto found = _stringMap.find(key);
            if (found == _stringMap.end()) return false;
            _stringMap.erase(found);
            return true;
        }

        void StringForEachValue(StringForEachValueFn* fn) override {
            for (auto& [key, value] : _stringMap) fn->invoke(key.c_str(), value);
        }

        Value* IntegerGetValue(std::int64_t key) override {
            auto found = _intMap.find(key);
            if (found == _intMap.end()) return Factories::NoneValue();
            return found->second;
        }

        void IntegerSetValue(std::int64_t key, Value* value) override { _intMap[key] = value; }

        bool IntegerRemoveValue(std::int64_t key) override {
            auto found = _intMap.find(key);
            if (found == _intMap.end()) return false;
            _intMap.erase(found);
            return true;
        }

        void IntegerForEachValue(IntegerForEachValueFn* fn) override {
            for (auto& [key, value] : _intMap) fn->invoke(key, value);
        }
    };
}
