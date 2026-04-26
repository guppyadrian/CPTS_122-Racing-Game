add_library(guppyNet
        include/network/ServerConnection.hpp
        src/ServerConnection.cpp
        include/network/Shared.hpp
        include/network/Packet.hpp
        src/Packet.cpp
        include/network/BufferParser.hpp)

target_sources(guppyNet PRIVATE "src/NetworkClient.cpp"
        "src/NetworkManager.cpp"
        "src/NetworkServer.cpp"
        "src/ReadBuffer.cpp"
        "src/Serialize.cpp")

target_include_directories(guppyNet PUBLIC include)

target_include_directories(guppyNet PUBLIC external/asio/include)
target_compile_definitions(guppyNet PUBLIC ASIO_STANDALONE)

target_include_directories(guppyNet PUBLIC external/json)