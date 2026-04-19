# CMakeList.txt : CMake project for PA9, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("PA9")

# Add source to this project's executable.
add_executable (PA9 src/PA9.cpp
        src/network/NetworkClient.cpp
        src/network/NetworkManager.cpp
        src/network/Serialize.cpp)

target_include_directories(PA9 PRIVATE include)

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