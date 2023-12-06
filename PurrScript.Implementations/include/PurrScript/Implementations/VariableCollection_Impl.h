#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <collections.h>
#include <function_pointer.h>

#include <string>

namespace PurrScript::Implementations {

    class VariableCollection_Impl : public VariableCollection {
        collections_map<std::string, Value*> _values;

        void MergeVariable(const char* name, Value* value) {
            if (value->IsNamespace()) {
                if (auto* namespaceValue = value->As<Namespace*>()) {
                    auto existingNamespace = _values.find(name);
                    if (existingNamespace != _values.end()) {
                        if (auto* existingNamespaceValue =
                                existingNamespace->second->As<Namespace*>()) {
                            existingNamespaceValue->GetVariables()->Merge(
                                namespaceValue->GetVariables()
                            );
                            return;
                        }
                    }
                }
            }
            _values.emplace(name, value->Copy());
        }

        FunctionPointer<void(const char*, Value*)> _mergeVariableFn =
            function_pointer(this, &VariableCollection_Impl::MergeVariable);

    public:
        VariableCollection_Impl(bool deleteVariablesOnDestruction = false) {
            _Log_("[+] VariableCollection_Impl");
        }

        ~VariableCollection_Impl() {
            _Log_("[~] VariableCollection_Impl");
            for (auto& [name, value] : _values) {
                if (value->IsDestructable()) {
                    _Log_("  [~] var {}", name);
                    delete value;
                }
            }
        }

        void AddVariable(const char* name, Value* value) override { _values.emplace(name, value); }

    public:
        bool RemoveVariable(const char* name) override {
            auto found = _values.find(name);
            if (found == _values.end()) return false;
            delete found->second;
            _values.erase(found);
            return true;
        }

        Value* GetValue(const char* name) override {
            if (auto it = _values.find(name); it != _values.end()) return it->second;
            return nullptr;
        }

        bool HasVariable(const char* name) override { return _values.contains(name); }

        void ForEachVariable(VariableCollection::ForEachVariableFn* fn) override {
            for (auto& [name, value] : _values) fn->invoke(name.c_str(), value);
        }

        void Merge(VariableCollection* other) override {
            other->ForEachVariable(&_mergeVariableFn);
        }
    };
}
