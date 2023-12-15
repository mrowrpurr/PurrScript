#pragma once

#include <function_pointer.h>

#include <cstdint>

namespace PurrScript {

    struct Server {
        using OnStartFn            = IFunctionPointer<void()>;
        using OnStopFn             = IFunctionPointer<void()>;
        using ConnectionIdentifier = void*;
        using ReplyFn              = IFunctionPointer<void(const char*)>;
        using OnMessageFn = IFunctionPointer<void(ConnectionIdentifier, const char*, ReplyFn*)>;

        virtual ~Server()                                    = default;
        virtual void Run()                                   = 0;
        virtual void Stop()                                  = 0;
        virtual void Send(ConnectionIdentifier, const char*) = 0;
        virtual void AddMessageListener(OnMessageFn*)        = 0;
        virtual void RemoveMessageListener(OnMessageFn*)     = 0;
        virtual void AddStartListener(OnStartFn*)            = 0;
        virtual void RemoveStartListener(OnStartFn*)         = 0;
        virtual void AddStopListener(OnStopFn*)              = 0;
        virtual void RemoveStopListener(OnStopFn*)           = 0;
    };
}
