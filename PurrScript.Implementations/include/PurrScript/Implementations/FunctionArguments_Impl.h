#pragma once

#include <PurrScript/API.h>
#include <_Log_.h>
#include <collections.h>

#include <memory>
#include <string>
#include <vector>

#include "Factories/FunctionArgument_Factory.h"
#include "FunctionArgument_Impl.h"

namespace PurrScript::Implementations {

    class FunctionArguments_Impl : public FunctionArguments {
        std::vector<std::unique_ptr<FunctionArgument_Impl>> _arguments;
        collections_map<std::string, FunctionArgument*>     _argumentsByName;

    public:
        FunctionArguments_Impl() { _Log_("[+] FunctionArguments_Impl"); }

        ~FunctionArguments_Impl() override { _Log_("[~] FunctionArguments_Impl"); }

        std::uint8_t GetArgumentCount() override { return _arguments.size(); }

        FunctionArgument* GetArgumentByIndex(std::uint8_t index) override {
            if (index >= _arguments.size()) return nullptr;
            return _arguments[index].get();
        }

        FunctionArgument* GetArgumentByName(const char* name) override {
            auto found = _argumentsByName.find(name);
            if (found == _argumentsByName.end()) return nullptr;
            return found->second;
        }

        void AddArgument(Value* value) {
            auto argument = Factories::CreateFunctionArgument(_arguments.size(), value);
            _arguments.emplace_back(std::move(argument));
        }

        void AddArgument(const char* name, Value* value) override {
            if (value == nullptr) {
                AddArgument(value);
                return;
            }
            auto argument = Factories::CreateFunctionArgument(name, _arguments.size(), value);
            _argumentsByName.emplace(name, argument.get());
            _arguments.emplace_back(std::move(argument));
        }

        void ForEachArgument(FunctionArguments::ForEachArgumentFn* callback) override {
            for (auto& argument : _arguments) callback->invoke(argument.get());
        }
    };
}
