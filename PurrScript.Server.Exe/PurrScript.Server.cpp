#include <PurrScript/Server/WebSocketServer.h>
#include <_Log_.h>
#include <string_format.h>

using namespace std;
using namespace PurrScript;

WebSocketServer server;

void OnMessage(Server::ConnectionIdentifier id, const char* message, Server::ReplyFn* reply) {
    _Log_("C++ Received Message: {}", message);
    reply->invoke(string_format("Hello from C++! You sent '{}'", message).c_str());
}

static auto OnMessageFn = function_pointer(OnMessage);

int main() {
    server.AddMessageListener(&OnMessageFn);
    server.Run();
    return 0;
}
