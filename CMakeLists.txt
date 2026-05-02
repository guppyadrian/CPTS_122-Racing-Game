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

set(FETCHCONTENT_UPDATES_DISCONNECTED OFF CACHE BOOL "" FORCE)
set(FETCHCONTENT_FULLY_DISCONNECTED OFF CACHE BOOL "" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets") # TODO: hey while this is nice... remember that copying a lot of assets may be slow
function(copy_assets target_name)
  add_custom_command(TARGET ${target_name} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/assets"
          "$<TARGET_FILE_DIR:${target_name}>/assets"
  )
endfunction()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

project ("PA9")

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)


# Add source to this project's executable.
add_executable (RacerGame
        src/PA9.cpp
        src/WallGenerator.cpp
        src/Player.cpp
        src/LevelLoader.cpp
        src/EndGoal.cpp
        src/TrackClock.cpp
        src/Countdown.cpp
        src/UI/MenuScene.cpp
        src/UI/UIButton.cpp
        src/UI/ButtonArray.cpp
        src/UI/LevelSelectScene.cpp
        src/UI/LobbyScene.cpp
        include/Multiplayer.hpp)

target_include_directories(RacerGame PRIVATE include)

add_subdirectory(lib/guppyNet)
add_subdirectory(lib/flowEngine)

# examples
add_subdirectory(examples)

# tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  include(Catch)
  enable_testing()
  add_subdirectory(test)
endif()

# Add assets folder
copy_assets(RacerGame)

# add libs

target_link_libraries(RacerGame flowEngine guppyNet)

install(TARGETS RacerGame DESTINATION .)

#target_compile_options(RacerGame PRIVATE -fsanitize=thread)
#target_link_options(RacerGame PRIVATE -fsanitize=thread)