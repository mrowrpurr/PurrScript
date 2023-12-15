#include <_Log_.h>

#include <csignal>
#include <cstdint>
#include <cxxopts.hpp>
#include <memory>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> WebSocketServer;

using namespace std;

unique_ptr<WebSocketServer> server = nullptr;

void on_sigint(int sig) {
    _Log_("Stopping server...");
    server->stop();
    _Log_("Server stopped.");
    exit(0);
}

void on_message(websocketpp::connection_hdl hdl, WebSocketServer::message_ptr msg) {
    _Log_("Received Message: {}", msg->get_payload());
}

void run_server(uint16_t port) {
    _Log_("Starting server...");
    server = make_unique<WebSocketServer>();
    server->init_asio();
    server->set_message_handler(&on_message);
    signal(SIGINT, &on_sigint);
    server->listen(port);
    server->start_accept();
    _Log_("Server started.");
    try {
        server->run();
    } catch (const exception& e) {
        _Log_("Error: {}", e.what());
    } catch (...) {
        _Log_("Unknown error.");
    }
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("PurrScript Server", "PurrScript WebSocket Server");
    options.allow_unrecognised_options();
    options.add_options()("h,help", "Print help")(
        "p,port", "Port", cxxopts::value<uint16_t>()->default_value("6969")
    );

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    run_server(result["port"].as<uint16_t>());

    return 0;
}