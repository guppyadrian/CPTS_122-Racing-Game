# CMakeList.txt : CMake project for PA9, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 4.0)

set(CMAKE_CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("PA9")

# Add source to this project's executable.
add_executable (PA9 "src/PA9.cpp")

target_include_directories(PA9 PRIVATE include)

# add libs
add_subdirectory(external/SFML)
add_subdirectory(external/box2d)

target_link_libraries(PA9 box2d SFML::Graphics SFML::Audio SFML::Network)

install(TARGETS PA9 DESTINATION .)