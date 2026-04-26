add_executable(FlowSetupExample main.cpp)

target_link_libraries(FlowSetupExample PRIVATE flowEngine)

copy_assets(FlowSetupExample)