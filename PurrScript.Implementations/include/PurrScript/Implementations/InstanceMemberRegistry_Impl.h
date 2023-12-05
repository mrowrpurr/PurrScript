#pragma once

#include <PurrScript/API.h>
#include <collections.h>

#include <string>

namespace PurrScript::Implementations {

    class InstanceMemberRegistry_Impl : public InstanceMemberRegistry {
        collections_map<std::string, collections_map<std::string, InstanceFunctionFn*>> _members;
        collections_map<InstanceFunctionFn*, std::unique_ptr<InstanceFunctionFn>>
            _managedInstanceFunctions;

    public:
        void AddMember(
            const char* typeName, const char* memberName, InstanceFunctionFn* invocationFn
        ) override {
            _members[typeName][memberName] = invocationFn;
        }

        void AddMember(
            const char* typeName, const char* memberName,
            std::unique_ptr<InstanceFunctionFn> invocationFn
        ) {
            auto invocationFnPtr                       = invocationFn.get();
            _managedInstanceFunctions[invocationFnPtr] = std::move(invocationFn);
            AddMember(typeName, memberName, invocationFnPtr);
        }

    private:
        // Note: in a Merge, we ONLY COPY POINTERS and not std::unique_ptr's
        // This means that if the original InstanceMemberRegistry is destroyed, the pointers
        // will be invalid
        void MergeMember(
            const char* typeName, const char* memberName, InstanceFunctionFn* invocationFn
        ) {
            _members[typeName][memberName] = invocationFn;
        }

        FunctionPointer<void(const char* typeName, const char* memberName, InstanceFunctionFn* fn)>
            _mergeFunction = function_pointer(this, &InstanceMemberRegistry_Impl::MergeMember);

    public:
        bool HasMember(const char* typeName, const char* memberName) override {
            auto foundMembers = _members.find(typeName);
            if (foundMembers == _members.end()) return false;
            auto foundMember = foundMembers->second.find(memberName);
            if (foundMember == foundMembers->second.end()) return false;
            return true;
        }

        InstanceFunctionFn* GetMemberFunction(const char* typeName, const char* memberName)
            override {
            auto foundMembers = _members.find(typeName);
            if (foundMembers == _members.end()) return nullptr;
            auto foundMember = foundMembers->second.find(memberName);
            if (foundMember == foundMembers->second.end()) return nullptr;
            return foundMember->second;
        }

        void ForEachMember(InstanceMemberRegistry::ForEachMemberFn* callback) override {
            for (auto& [typeName, members] : _members)
                for (auto& [memberName, invocationFn] : members)
                    callback->invoke(typeName.c_str(), memberName.c_str(), invocationFn);
        }

        void ForEachMember(const char* typeName, InstanceMemberRegistry::ForEachMemberFn* callback)
            override {
            auto foundMembers = _members.find(typeName);
            if (foundMembers == _members.end()) return;
            for (auto& [memberName, invocationFn] : foundMembers->second)
                callback->invoke(typeName, memberName.c_str(), invocationFn);
        }

        void Merge(InstanceMemberRegistry* other) override {
            other->ForEachMember(&_mergeFunction);
        }
    };
}
