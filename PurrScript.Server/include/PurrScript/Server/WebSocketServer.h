#pragma once

#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


namespace PurrScript::Server {

    class WebSocketServer {
        typedef websocketpp::server<websocketpp::config::asio> WebSocketServerPP;

        std::uint16_t     _port;
        WebSocketServerPP _server;

    public:
        WebSocketServer(std::uint16_t port = 6969) : _port(port) {}

        void

        // void run();
    };
}