#pragma once

#include <PurrScript/Server.h>
#include <_Log_.h>
#include <string_format.h>

namespace PurrScript {

    class ServerMessageController {
        void OnMessage(
            Server::ConnectionIdentifier id, const char* message, Server::ReplyFn* reply
        ) {
            _Log_("C++ *controller* Received Message: {}", message);
            // reply->invoke(string_format("Hello from C++! You sent '{}'", message).c_str());
        }

        std::unique_ptr<
            FunctionPointer<void(Server::ConnectionIdentifier, const char*, Server::ReplyFn*)>>
            _onMessageFn = unique_function_pointer(this, &ServerMessageController::OnMessage);

    public:
        Server::OnMessageFn* OnMessageFn() const { return _onMessageFn.get(); }
    };
}
