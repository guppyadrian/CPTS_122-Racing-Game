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

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets") # TODO: hey while this is nice... remember that copying a lot of assets may be slow
function(copy_assets target_name)
  add_custom_command(TARGET ${target_name} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/assets"
          "$<TARGET_FILE_DIR:${target_name}>/assets"
  )
endfunction()

project ("PA9")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()


# Add source to this project's executable.
add_executable (RacerGame src/PA9.cpp src/WallGenerator.cpp src/Player.cpp src/LevelLoader.cpp)

target_include_directories(RacerGame PRIVATE include)

add_subdirectory(lib/guppyNet)
add_subdirectory(lib/flowEngine)

# examples
add_subdirectory(examples)

# Add assets folder
copy_assets(RacerGame)

# I heard this is supposed to help with SFML fetching from github all the time
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "" FORCE)
set(FETCHCONTENT_FULLY_DISCONNECTED ON CACHE BOOL "" FORCE)

# add libs

target_link_libraries(RacerGame flowEngine guppyNet)

install(TARGETS RacerGame DESTINATION .)