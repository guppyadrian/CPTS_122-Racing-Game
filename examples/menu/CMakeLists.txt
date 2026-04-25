add_executable(NetworkExample main.cpp MainMenu.cpp MainMenu.hpp)

target_link_libraries(NetworkExample PRIVATE guppyNet)

//target_include_directories({CMAKE_SOURCE_DIR}/include)