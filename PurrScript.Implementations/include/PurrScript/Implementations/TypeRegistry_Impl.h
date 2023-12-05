#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <collections.h>

#include <string>

namespace PurrScript::Implementations {

    class TypeRegistry_Impl : public TypeRegistry {
        collections_set<std::string>                               _types;
        collections_map<std::string, collections_set<std::string>> _parentTypes;

    public:
        std::uint32_t GetTypeCount() override { return _types.size(); }

        void RegisterType(const char* name) override { _types.insert(name); }

        void AddParentType(const char* childName, const char* parentName) override {
            _parentTypes[childName].insert(parentName);
            if (_types.find(parentName) == _types.end()) _types.insert(parentName);
            if (_types.find(childName) == _types.end()) _types.insert(childName);
        }

    private:
        FunctionPointer<void(const char*)> _registerTypeFn =
            function_pointer(this, &TypeRegistry_Impl::RegisterType);
        FunctionPointer<void(const char*, const char*)> _addParentTypeFn =
            function_pointer(this, &TypeRegistry_Impl::AddParentType);

    public:
        bool IsType(const char* name) override { return _types.find(name) != _types.end(); }

        void ForEachType(ForEachCallbackFn* callback) override {
            for (auto& type : _types) callback->invoke(type.c_str());
        }

        void ForEachParent(const char* name, ForEachCallbackFn* callback) override {
            auto foundParents = _parentTypes.find(name);
            if (foundParents == _parentTypes.end()) return;
            for (auto& parent : foundParents->second) callback->invoke(parent.c_str());
        }

        void ForEachTypeAndParent(ForEachTypeAndParentCallbackFn* callback) override {
            for (auto& [type, parents] : _parentTypes)
                for (auto& parent : parents) callback->invoke(type.c_str(), parent.c_str());
        }

        void Merge(TypeRegistry* other) override {
            other->ForEachType(&_registerTypeFn);
            other->ForEachTypeAndParent(&_addParentTypeFn);
        }
    };
}
