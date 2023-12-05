#pragma once

#include <PurrScript/API.h>
#include <collections.h>

#include <string>

namespace PurrScript::Implementations {

    class StaticMemberRegistry_Impl : public StaticMemberRegistry {
        collections_map<std::string, collections_map<std::string, StaticFunctionFn*>> _members;
        collections_map<StaticFunctionFn*, std::unique_ptr<StaticFunctionFn>>
            _managedMemberFunctions;

    public:
        void AddMember(const char* typeName, const char* memberName, StaticFunctionFn* invocationFn)
            override {
            _members[typeName][memberName] = invocationFn;
        }

        void AddMember(
            const char* typeName, const char* memberName,
            std::unique_ptr<StaticFunctionFn> invocationFn
        ) {
            auto invocationFnPtr                     = invocationFn.get();
            _managedMemberFunctions[invocationFnPtr] = std::move(invocationFn);
            AddMember(typeName, memberName, invocationFnPtr);
        }

    private:
        // Note: in a Merge, we ONLY COPY POINTERS and not std::unique_ptr's
        // This means that if the original StaticMemberRegistry is destroyed, the pointers
        // will be invalid
        void MergeMember(
            const char* typeName, const char* memberName, StaticFunctionFn* invocationFn
        ) {
            _members[typeName][memberName] = invocationFn;
        }

        FunctionPointer<void(const char* typeName, const char* memberName, StaticFunctionFn* fn)>
            _mergeFunction = function_pointer(this, &StaticMemberRegistry_Impl::MergeMember);

    public:
        bool HasMember(const char* typeName, const char* memberName) override {
            auto foundMembers = _members.find(typeName);
            if (foundMembers == _members.end()) return false;
            auto foundMember = foundMembers->second.find(memberName);
            if (foundMember == foundMembers->second.end()) return false;
            return true;
        }

        StaticFunctionFn* GetMemberFunction(const char* typeName, const char* memberName) override {
            auto foundMembers = _members.find(typeName);
            if (foundMembers == _members.end()) return nullptr;
            auto foundMember = foundMembers->second.find(memberName);
            if (foundMember == foundMembers->second.end()) return nullptr;
            return foundMember->second;
        }

        void ForEachMember(StaticMemberRegistry::ForEachMemberFn* callback) override {
            for (auto& [typeName, members] : _members)
                for (auto& [memberName, invocationFn] : members)
                    callback->invoke(typeName.c_str(), memberName.c_str(), invocationFn);
        }

        void ForEachMember(const char* typeName, StaticMemberRegistry::ForEachMemberFn* callback)
            override {
            auto foundMembers = _members.find(typeName);
            if (foundMembers == _members.end()) return;
            for (auto& [memberName, invocationFn] : foundMembers->second)
                callback->invoke(typeName, memberName.c_str(), invocationFn);
        }

        void Merge(StaticMemberRegistry* other) override { other->ForEachMember(&_mergeFunction); }
    };
}
