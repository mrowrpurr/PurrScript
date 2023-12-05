#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>

#include <string>
#include <string_view>

#include "Value_Impl.h"
#include "VariableCollection_Impl.h"


namespace PurrScript::Implementations {

    class Namespace_Impl : public Namespace, public Value_Impl {
        std::string             _name;
        VariableCollection_Impl _variables;

    public:
        Namespace_Impl(std::string_view name, bool isDestructable = true)
            : Value_Impl(isDestructable), _name(name) {
            _Log_("[+] Namespace_Impl {}", _name);
        }

        ~Namespace_Impl() override { _Log_("[~] Namespace_Impl {}", _name); }

        const char* GetTypeName() override { return Namespace::TYPE_NAME; }
        bool        IsDestructable() override { return Value_Impl::IsDestructable(); }
        void        SetIsDestructable(bool isDestuctable = true) override {
            Value_Impl::SetIsDestructable(isDestuctable);
        }

        const char*         GetName() override { return _name.c_str(); }
        VariableCollection* GetVariables() override { return &_variables; }

        Value* Copy() override {
            auto* theNamespace = new Namespace_Impl(_name, Value_Impl::_isDestuctable);
            theNamespace->GetVariables()->Merge(&_variables);
            return theNamespace;
        }

        Namespace_Impl* DefineChildNamespace(std::string_view childNamespaceName) {
            auto* childNamespace = new Namespace_Impl(childNamespaceName);
            _variables.AddVariable(childNamespaceName.data(), childNamespace);
            return childNamespace;
        }
    };
}
