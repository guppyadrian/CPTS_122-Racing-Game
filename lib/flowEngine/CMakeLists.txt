add_library(flowEngine)

target_sources(flowEngine PRIVATE 
        src/GameObject.cpp
        src/LevelScene.cpp
        src/PhysicsManager.cpp
        src/Renderer.cpp
        src/Rigidbody.cpp
        src/Scene.cpp
        src/SceneManager.cpp
        src/SpriteRenderer.cpp
        src/Transform.cpp 
        src/Camera.cpp
        src/ParticleSystem.cpp)

target_include_directories(flowEngine PUBLIC include)


add_subdirectory(external/SFML)
add_subdirectory(external/box2d)

target_link_libraries(flowEngine PUBLIC box2d SFML::Graphics)