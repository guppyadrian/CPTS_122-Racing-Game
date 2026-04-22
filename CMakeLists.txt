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

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()


# Add source to this project's executable.
add_executable (PA9 src/PA9.cpp)

target_include_directories(PA9 PRIVATE include)

add_subdirectory(lib/guppyNet)
add_subdirectory(lib/flowEngine)

# examples
add_subdirectory(examples)

# Add assets folder
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets") # TODO: hey while this is nice... remember that copying a lot of assets may be slow
add_custom_command(                          #       you could swap back to cmake style folder copying, but remember to regenerate CMAKE!!!
        OUTPUT "${CMAKE_BINARY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
        DEPENDS "${SOURCE_DIR}"
)
add_custom_target(copy_mydata ALL DEPENDS "${CMAKE_BINARY_DIR}")

# I heard this is supposed to help with SFML fetching from github all the time
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "" FORCE)
set(FETCHCONTENT_FULLY_DISCONNECTED ON CACHE BOOL "" FORCE)

# add libs

target_link_libraries(PA9 flowEngine guppyNet)

install(TARGETS PA9 DESTINATION .)