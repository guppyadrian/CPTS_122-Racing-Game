# CMakeList.txt : CMake project for PA9, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("PA9")

# Add source to this project's executable.
add_executable (PA9 "src/PA9.cpp" "include/PA9.h")

target_include_directories(PA9 PRIVATE include)

# add libs
add_subdirectory(external/box2d)
add_subdirectory(external/SFML)

target_link_libraries(PA9 box2d SFML::Graphics)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PA9 PROPERTY CXX_STANDARD 20)
endif()

install(TARGETS PA9 DESTINATION .)