#pragma once

#include <PurrScript/API.h>

#include <memory>
#include <string>
#include <string_view>

namespace PurrScript::Implementations {

    class ExecuteCodeResult_Impl : public ExecuteCodeResult {
        Status                       _status  = Status::Unset;
        Value*                       _result  = nullptr;
        std::unique_ptr<std::string> _message = nullptr;

    public:
        ExecuteCodeResult_Impl(Status status) : _status(status) {}
        ExecuteCodeResult_Impl(Status status, std::string_view message)
            : _status(status), _message(std::make_unique<std::string>(message)) {}
        ExecuteCodeResult_Impl(Value* result, Status status) : _result(result), _status(status) {}
        ExecuteCodeResult_Impl(Value* result, Status status, std::string_view message)
            : _result(result), _status(status), _message(std::make_unique<std::string>(message)) {}

        Status GetStatus() const override { return _status; }
        void   SetStatus(Status status) override { _status = status; }

        const char* GetErrorMessage() const override {
            if (_message == nullptr) return nullptr;
            return _message->c_str();
        }
        void SetErrorMessage(const char* message) override {
            if (_message == nullptr) _message = std::make_unique<std::string>(message);
            else *_message = message;
        }

        Value* GetResult() const override { return _result; }
        void   SetResult(Value* result) override { _result = result; }

        static ExecuteCodeResult* OK(Value* result) {
            return new ExecuteCodeResult_Impl(result, Status::OK);
        }

        static ExecuteCodeResult* SyntaxError(Value* result, std::string_view message) {
            return new ExecuteCodeResult_Impl(Status::SyntaxError, message);
        }

        static ExecuteCodeResult* RuntimeError(std::string_view message) {
            return new ExecuteCodeResult_Impl(Status::RuntimeError, message);
        }

        static ExecuteCodeResult* Exception(std::string_view message) {
            return new ExecuteCodeResult_Impl(Status::Exception, message);
        }

        static ExecuteCodeResult* UnknownException() {
            return new ExecuteCodeResult_Impl(Status::Exception);
        }
    };
}
