//
// Created by guppy on 4/22/26.
//

#include <iostream>
#include <network/NetworkServer.hpp>

using namespace gp::network;

int main()
{
    NetworkManager::Start();

    NetworkServer server;

    server.onConnection = [&server](Socket socket)
    {
        socket->on<std::string>("exampleEvent", [socket](const std::string &data)
        {
            std::cout << "got an event: " << data << std::endl;
            socket->emit("exampleEvent", data);
        });
        
        //socket->emit("exampleEvent", "Isn't this cool?");

        socket->on<std::array<float, 3>>("playerUpdate", [&](std::array<float, 3> data)
        {
            std::cout << "got pos: (" << data[0] << ", " << data[1] << ", " << data[2] << ")" << std::endl;
            ByteBuffer buffer;
            
            constexpr int32_t id = 1;
            const auto idBytes = reinterpret_cast<const uint8_t*>(&id);
            buffer.insert(buffer.end(), idBytes, idBytes + sizeof(id));

            const auto dataBytes = reinterpret_cast<const uint8_t*>(data.data());
            buffer.insert(buffer.end(), dataBytes, dataBytes + data.size() * sizeof(float));
            
            server.emit("playerUpdate", buffer);
        });
    };

    server.listen(25550);

    while (true)
    {
        
    }
}
