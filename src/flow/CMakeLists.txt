add_library(flowEngine)

target_sources(flowEngine PRIVATE
        "Transform.cpp"
        "GameObject.cpp"
        "Renderer.cpp"
        "SpriteRenderer.cpp"
        "Rigidbody.cpp"
        "PhysicsManager.cpp"
        "Scene.cpp"
        "SceneManager.cpp"
        "LevelScene.cpp")

target_include_directories(flowEngine
        PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(flowEngine SFML::Graphics box2d)