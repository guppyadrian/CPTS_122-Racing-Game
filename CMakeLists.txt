# CMakeList.txt : CMake project for PA9, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 4.0.2)

set(CMAKE_CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project ("PA9")

set(PA9_SOURCES
    "src/PA9.cpp"
    "src/Transform.cpp"
    "src/GameObject.cpp"
    "src/Renderer.cpp"
    "src/SpriteRenderer.cpp"
    "src/Rigidbody.cpp"
    "src/PhysicsManager.cpp"
    "src/Scene.cpp"
    "src/SceneManager.cpp"
    "src/LevelScene.cpp")

# Add source to this project's executable.
add_executable (PA9 ${PA9_SOURCES} src/PA9.cpp
        src/network/NetworkClient.cpp
        src/network/NetworkManager.cpp
        src/network/Serialize.cpp
        src/network/ReadBuffer.cpp
        src/network/NetworkServer.cpp)

target_include_directories(PA9 PRIVATE include)

# Add assets folder
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# add libs
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "" FORCE)
set(FETCHCONTENT_FULLY_DISCONNECTED ON CACHE BOOL "" FORCE)
add_subdirectory(external/SFML)
add_subdirectory(external/box2d)

target_include_directories(PA9 PRIVATE external/asio/include)
target_compile_definitions(PA9 PRIVATE ASIO_STANDALONE)

target_include_directories(PA9 PRIVATE external/json)

target_link_libraries(PA9 box2d SFML::Graphics)

install(TARGETS PA9 DESTINATION .)