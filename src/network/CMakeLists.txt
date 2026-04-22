add_library(guppyNet)

target_sources(guppyNet PRIVATE
        "NetworkClient.cpp"
        "NetworkManager.cpp"
        "NetworkServer.cpp"
        "ReadBuffer.cpp"
        "Serialize.cpp")

target_include_directories(guppyNet
        PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(guppyNet PRIVATE ${CMAKE_SOURCE_DIR}/external/asio/include)
target_compile_definitions(guppyNet PRIVATE ASIO_STANDALONE)

target_include_directories(guppyNet PRIVATE ${CMAKE_SOURCE_DIR}/external/json)