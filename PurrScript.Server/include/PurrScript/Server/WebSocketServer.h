#pragma once

#include <PurrScript/Server.h>
#include <_Log_.h>
#include <collections.h>

#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/endpoint.hpp>
#include <websocketpp/server.hpp>

namespace PurrScript {

    class WebSocketServer : public Server {
        typedef websocketpp::server<websocketpp::config::asio>     ServerType;
        typedef websocketpp::connection<websocketpp::config::asio> ConnectionPtr;

        std::uint16_t _port;
        ServerType    _server;

        collections_set<OnMessageFn*> _message_listeners;
        collections_set<OnStartFn*>   _start_listeners;
        collections_set<OnStopFn*>    _stop_listeners;

        collections_map<ConnectionIdentifier, std::shared_ptr<ConnectionPtr>> _connectionPointers;

        inline void OnMessage(websocketpp::connection_hdl hdl, ServerType::message_ptr msg) {
            auto connectionPtr = _server.get_con_from_hdl(hdl);
            auto connectionId  = static_cast<ConnectionIdentifier>(connectionPtr.get());

            if (_connectionPointers.find(connectionId) == _connectionPointers.end())
                _connectionPointers.emplace(connectionId, connectionPtr);

            auto reply = unique_function_pointer([connectionId, this](const char* message) {
                Send(connectionId, message);
            });

            for (auto& listener : _message_listeners)
                listener->invoke(hdl.lock().get(), msg->get_payload().c_str(), reply.get());
        }

    public:
        WebSocketServer(std::uint16_t port = 6969) : _port(port) {}

        std::uint16_t port() const { return _port; }

        void Run() override {
            _server.init_asio();
            _server.set_message_handler([this](auto hdl, auto msg) { OnMessage(hdl, msg); });
            _server.listen(_port);
            _server.start_accept();
            for (auto& listener : _start_listeners) listener->invoke();
            _server.run();
            for (auto& listener : _stop_listeners) listener->invoke();
        }

        void Stop() override { _server.stop(); }

        void Send(ConnectionIdentifier connectionId, const char* message) override {
            auto foundConnection = _connectionPointers.find(connectionId);
            if (foundConnection != _connectionPointers.end())
                _server.send(foundConnection->second, message, websocketpp::frame::opcode::text);
            else _Log_("Connection not found.");
        }

        void AddMessageListener(OnMessageFn* listener) override {
            _message_listeners.insert(listener);
        }
        void RemoveMessageListener(OnMessageFn* listener) override {
            _message_listeners.erase(listener);
        }
        void AddStartListener(OnStartFn* listener) override { _start_listeners.insert(listener); }
        void RemoveStartListener(OnStartFn* listener) override { _start_listeners.erase(listener); }
        void AddStopListener(OnStopFn* listener) override { _stop_listeners.insert(listener); }
        void RemoveStopListener(OnStopFn* listener) override { _stop_listeners.erase(listener); }
    };
}
