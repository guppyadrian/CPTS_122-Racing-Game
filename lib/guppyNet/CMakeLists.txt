add_library(guppyNet)

target_sources(guppyNet PRIVATE "src/NetworkClient.cpp"
        "src/NetworkManager.cpp"
        "src/NetworkServer.cpp"
        "src/ReadBuffer.cpp"
        "src/Serialize.cpp")

target_include_directories(guppyNet PUBLIC include)

target_include_directories(guppyNet PUBLIC external/asio/include)
target_compile_definitions(guppyNet PUBLIC ASIO_STANDALONE)

target_include_directories(guppyNet PUBLIC external/json)