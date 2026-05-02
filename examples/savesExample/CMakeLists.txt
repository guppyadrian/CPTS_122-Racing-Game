cmake_minimum_required(VERSION 3.16)
project(savesExample)

set(CMAKE_CXX_STANDARD 17)

add_executable(savesExample
        main.cpp
        PlayerData.cpp
)

target_include_directories(savesExample PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# copy sav.csv to build folder every build
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sav.csv
        ${CMAKE_CURRENT_BINARY_DIR}/sav.csv COPYONLY)