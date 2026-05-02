//
// Created by guppy on 4/18/2026.
//

#include "network/NetworkManager.hpp"

namespace gp::network
{
    void NetworkManager::Start()
    {
        if (_instance != nullptr) throw std::runtime_error("NetworkManager already instantiated!");
        _running = true;
        _instance.reset(new NetworkManager());
    }

    NetworkManager::NetworkManager() : _guard(asio::make_work_guard(_io))
    {
        _thread = std::thread(&asio::io_context::run, &_io);
    }

    void NetworkManager::stop()
    {
        if (!_running) throw std::runtime_error("NetworkManager isn't running!");

        _running = false;
        _guard.reset();
        if (_thread.joinable()) _thread.join();
    }

    asio::io_context& NetworkManager::io()
    {
        if (!_running) throw std::runtime_error("NetworkManager isn't running!");
        return _instance->_io;
    }
} // gp