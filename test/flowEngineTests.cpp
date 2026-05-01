#include <catch2/catch_test_macros.hpp>

#include <flow/GameObject.hpp>
#include <iostream>

#include "flow/LevelScene.hpp"
#include "flow/SceneManager.hpp"
#include "flow/components/NetworkEmitter.hpp"
#include "flow/components/NetworkGhost.hpp"
#include "flow/components/Rigidbody.hpp"

// Jonathon made test cases using Claude, Adrian modified them to use Catch2

TEST_CASE( "GameObject Construction", "[gameObject][flowEngine]" )
{
    flow::GameObject gameObject;
    
    REQUIRE(&gameObject.mTransform != nullptr); // CLion says this will never be null so it is a pointless comparison
    REQUIRE(gameObject.mTransform.getPosition().x == 0.0f);
    REQUIRE(gameObject.mTransform.getPosition().y == 0.0f);
}

TEST_CASE( "GameObject Transform Manipulation", "[gameObject][flowEngine]" )
{
    flow::GameObject gameObject;
    sf::Vector2f testPos(100.0f, 50.0f);

    gameObject.mTransform.setPosition(testPos);
    REQUIRE(gameObject.mTransform.getPosition().x == testPos.x);
    REQUIRE(gameObject.mTransform.getPosition().y == testPos.y);

    gameObject.mTransform.setRotationDeg(45.0f);
    REQUIRE(gameObject.mTransform.getRotationDeg() == 45.0f);

    sf::Vector2f testScale(2.0f, 3.0f);
    gameObject.mTransform.setScale(testScale);
    REQUIRE(gameObject.mTransform.getScale().x == testScale.x);
    REQUIRE(gameObject.mTransform.getScale().y == testScale.y);
}

TEST_CASE( "GameObject Move Sematics ?", "[gameObject][flowEngine]" )
{
    flow::GameObject original;
    original.mTransform.setPosition(sf::Vector2f(50.0f, 75.0f));

    flow::GameObject moved = std::move(original);
    REQUIRE(moved.mTransform.getPosition().x == 50.0f);
    REQUIRE(moved.mTransform.getPosition().y == 75.0f);
}

TEST_CASE( "GameObject With RigidBody", "[gameObject][component][rigidbody][flowEngine]" )
{
    flow::GameObject gameObject;

    // Add a Rigidbody component
    auto& rbComponent = gameObject.addComponent<flow::Rigidbody>();

    // Verify component was added
    auto* retrievedComponent = gameObject.getComponent<flow::Rigidbody>();
    REQUIRE(retrievedComponent != nullptr);
    REQUIRE(&rbComponent == retrievedComponent);
}

TEST_CASE( "GameObject Component Pointer ?", "[gameObject][component][rigidbody][flowEngine]" )
{
    flow::GameObject gameObject;

    auto& rbComponent = gameObject.addComponent<flow::Rigidbody>();

    // Verify that the component's mGameObject pointer is set correctly
    REQUIRE(rbComponent.mGameObject == &gameObject);
}

TEST_CASE( "Scene Construction", "[scene][gameObject][flowEngine]" )
{
    auto scene = std::make_unique<flow::LevelScene>("test_scene");

    // Create and add a game object
    flow::GameObject testObject;
    testObject.mTransform.setPosition(sf::Vector2f(10.0f, 20.0f));

    auto& addedObject = scene->AddGameObject(std::move(testObject));
    REQUIRE(addedObject.mTransform.getPosition().x == 10.0f);
    REQUIRE(addedObject.mTransform.getPosition().y == 20.0f);
}

TEST_CASE( "Scene Lifecycle", "[flowEngine][levelScene]" )
{
    auto scene = std::make_unique<flow::LevelScene>("lifecycle_test");
    
    scene->onEnter(); // fails if these lines throw ... ? (idk if these throw an actual error or just have like segfault or smthn
    scene->onExit();
}

TEST_CASE( "SceneManager Singleton", "[sceneManager][flowEngine]" )
{
    flow::SceneManager& manager1 = flow::SceneManager::getGlobal();
    flow::SceneManager& manager2 = flow::SceneManager::getGlobal();

    REQUIRE(&manager1 == &manager2);
}

TEST_CASE( "SceneManager Load Scene", "[scene][sceneManager][flowEngine]" )
{
    flow::SceneManager& manager = flow::SceneManager::getGlobal();

    auto newScene = std::make_unique<flow::LevelScene>("test_load_scene_9");

    // Load scene
    bool loadSuccess = manager.loadScene(std::move(newScene));
    REQUIRE(loadSuccess);

    // Switch to scene
    bool switchSuccess = manager.switchScene("test_load_scene_9");
    REQUIRE(switchSuccess);

    // Verify current scene
    flow::Scene& currentScene = manager.getCurrentScene();
    REQUIRE(currentScene.get_uuid() == "test_load_scene_9");
}

TEST_CASE( "SceneManager Remove Scene", "[scene][sceneManager][flowEngine]" )
{
    flow::SceneManager& manager = flow::SceneManager::getGlobal();

    auto sceneToRemove = std::make_unique<flow::LevelScene>("scene_to_remove_10");
    manager.loadScene(std::move(sceneToRemove));

    // Remove the scene
    bool removeSuccess = manager.removeScene("scene_to_remove_10");
    REQUIRE(removeSuccess);
}

TEST_CASE( "NetworkEmitter", "[flowEngine][networkEmitter][network]" )
{
    flow::GameObject networkObject;
    std::string eventName = "player_position_update";

    auto& emitter = networkObject.addComponent<flow::NetworkEmitter>(eventName);

    // Verify component was added
    auto* retrievedEmitter = networkObject.getComponent<flow::NetworkEmitter>();
    REQUIRE(retrievedEmitter != nullptr);
    REQUIRE(&emitter == retrievedEmitter);
}

TEST_CASE( "NetworkGhost", "[flowEngine][networkGhost][network]" )
{
    flow::GameObject ghostObject;
    int ghostId = 42;

    auto& ghost = ghostObject.addComponent<flow::NetworkGhost>(ghostId);

    // Verify component was added
    auto* retrievedGhost = ghostObject.getComponent<flow::NetworkGhost>();
    REQUIRE(retrievedGhost != nullptr);
    REQUIRE(&ghost == retrievedGhost);
}

TEST_CASE( "Multiple NetworkEmitters", "[flowEngine][networkEmitter][network]" )
{
    flow::GameObject object;

    std::string eventNames[] = {
        "position_sync",
        "rotation_sync",
        "velocity_sync",
        "state_change"
    };

    for (int i = 0; i < 4; i++) {
        auto& emitter = object.addComponent<flow::NetworkEmitter>(eventNames[i]);
        REQUIRE(object.getComponent<flow::NetworkEmitter>() != nullptr);
    }
}

TEST_CASE( "NetworkGhost & Rigidbody", "[flowEngine][networkGhost][network][rigidbody]" )
{
    flow::GameObject ghostPlayer;
    ghostPlayer.mTransform.setPosition(sf::Vector2f(250.0f, 150.0f));

    auto& rb = ghostPlayer.addComponent<flow::Rigidbody>();
    auto& ghost = ghostPlayer.addComponent<flow::NetworkGhost>(1);

    REQUIRE(ghostPlayer.getComponent<flow::Rigidbody>() != nullptr);
    REQUIRE(ghostPlayer.getComponent<flow::NetworkGhost>() != nullptr);
}

TEST_CASE( "NetworkGhosts in Scene", "[flowEngine][network][networkGhost][scene]" )
{
    auto scene = std::make_unique<flow::LevelScene>("network_ghost_scene");

    // Add multiple ghost players
    for (int i = 0; i < 4; i++) {
        flow::GameObject ghostPlayer;
        ghostPlayer.mTransform.setPosition(sf::Vector2f(100.0f + (i * 60.0f), 200.0f));

        auto& ghost = ghostPlayer.addComponent<flow::NetworkGhost>(i + 1);
        auto& rb = ghostPlayer.addComponent<flow::Rigidbody>();

        scene->AddGameObject(std::move(ghostPlayer));
    }
}

