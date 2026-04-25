//
// Created by guppy on 4/22/26.
//

#include <iostream>
#include <network/NetworkServer.hpp>

using namespace gp::network;

int main()
{
    NetworkManager::Start();

    NetworkServer server(3000);

    server.onConnection = [](Socket socket)
    {
        socket->on<std::string>("exampleEvent", [socket](const std::string &data)
        {
            std::cout << "got an event: " << data << std::endl;
            socket->emit("exampleEvent", data);
        });
        
        socket->emit("exampleEvent", "Isn't this cool?");
    };

    server.listen();

    while (true)
    {
        
    }
}
