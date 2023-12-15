#include <PurrScript/Server/ServerMessageController.h>
#include <PurrScript/Server/WebSocketServer.h>

using namespace std;
using namespace PurrScript;

WebSocketServer         server;
ServerMessageController controller;

int main() {
    server.AddMessageListener(controller.OnMessageFn());
    server.Run();
    return 0;
}
