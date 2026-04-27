add_executable(MenuExample main.cpp MainMenu.cpp MainMenu.hpp
        LevelMenu.cpp
        LevelMenu.hpp
        MainMenu.cpp
        MainMenu.hpp
        MenuDrawable.cpp
        MenuDrawable.hpp)


target_link_libraries(MenuExample PRIVATE SFML::Graphics flowEngine)

target_include_directories(MenuExample PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_custom_command(TARGET MenuExample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../../assets
        $<TARGET_FILE_DIR:MenuExample>/assets
)