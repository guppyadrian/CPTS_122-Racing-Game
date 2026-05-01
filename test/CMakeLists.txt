add_executable(ExampleTest1 exampleTest.cpp)
add_test(NAME ExampleTest1 COMMAND ExampleTest1)

add_executable(ExampleTest2 exampleTest2.cpp)
add_test(NAME ExampleTest2 COMMAND ExampleTest2)

add_executable(tests tests.cpp)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests flowEngine guppyNet)
add_test(NAME tests COMMAND tests)

add_executable(tests_advanced tests_advanced.cpp 
               ${CMAKE_SOURCE_DIR}/src/WallGenerator.cpp)
target_include_directories(tests_advanced PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests_advanced flowEngine guppyNet)
add_test(NAME tests_advanced COMMAND tests_advanced)