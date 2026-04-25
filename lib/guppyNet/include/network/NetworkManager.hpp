//
// Created by guppy on 4/18/2026.
//

#pragma once

#include <thread>

#include "asio/io_context.hpp"

namespace gp::network
{
    class NetworkManager
    {
    private:
        asio::io_context _io;
        asio::executor_work_guard<asio::io_context::executor_type> _guard;
        std::thread _thread;

        static inline bool _running = false; // TODO: is this used?
        static inline std::unique_ptr<NetworkManager> _instance = nullptr;
    public:
        // Starts running networking in another thread
        // NOTE: please remember to call Stop()!
        static void Start();
        static void Stop() { _instance->stop(); }

        NetworkManager(const NetworkManager&) = delete;
        NetworkManager& operator=(const NetworkManager&) = delete;
        // ts ^^^ is NOT allowed!

        static asio::io_context& io();
    private:
        NetworkManager();
        void stop();
    };
} // gp