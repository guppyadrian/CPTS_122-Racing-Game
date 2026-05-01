#include <cassert>
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <flow/GameObject.hpp>
#include <flow/Component.hpp>
#include <flow/SceneManager.hpp>
#include <flow/LevelScene.hpp>
#include <flow/components/Rigidbody.hpp>
#include <flow/PhysicsManager.hpp>

#include "WallGenerator.hpp"
#include "flow/components/NetworkEmitter.hpp"
#include "flow/components/NetworkGhost.hpp"


/*
This File is written entirely using Generative AI (specifically Claude)
*/
// Helper macro for assertions
#define ASSERT_TRUE(condition) assert((condition) && "Assertion failed: " #condition)
#define ASSERT_EQ(a, b) assert((a) == (b) && "Assertion failed: values not equal")
#define ASSERT_NE(a, b) assert((a) != (b) && "Assertion failed: values are equal")
#define ASSERT_NEAR(a, b, tolerance) assert(std::abs((a) - (b)) < (tolerance) && "Assertion failed: values differ by more than tolerance")

// ===== WallGenerator Tests =====

// Test 1: WallGenerator linear wall creation
bool test_WallGeneratorLinearWall() {
    std::cout << "Running Test 1: WallGenerator linear wall creation..." << std::endl;

    sf::Vector2f wallPos(200.0f, 300.0f);
    int wallLength = 50;
    float wallAngle = 45.0f;
    sf::Color wallColor = sf::Color::Red;

    flow::GameObject wall = WallGenerator::GenerateWall(wallPos, wallLength, wallAngle, wallColor);

    // Verify wall was created with correct properties
    ASSERT_NE(wall.getComponent<flow::Rigidbody>(), nullptr);
    ASSERT_EQ(wall.mTransform.getPosition().x, wallPos.x);
    ASSERT_EQ(wall.mTransform.getPosition().y, wallPos.y);
    ASSERT_NEAR(wall.mTransform.getRotationDeg(), wallAngle, 0.1f);

    std::cout << "  ✓ Test 1 passed!" << std::endl;
    return true;
}

// Test 2: WallGenerator arc wall creation
bool test_WallGeneratorArcWall() {
    std::cout << "Running Test 2: WallGenerator arc wall creation..." << std::endl;

    sf::Vector2f arcPos(150.0f, 250.0f);
    int radius = 100;
    float startAngle = 0.0f;
    float arcLength = 1.57f; // 90 degrees in radians
    int subdivisions = 8;
    sf::Color arcColor = sf::Color::Blue;

    flow::GameObject arcWall = WallGenerator::GenerateWall(arcPos, radius, startAngle, arcLength, subdivisions, arcColor);

    // Verify arc wall was created with correct properties
    ASSERT_NE(arcWall.getComponent<flow::Rigidbody>(), nullptr);
    ASSERT_EQ(arcWall.mTransform.getPosition().x, arcPos.x);
    ASSERT_EQ(arcWall.mTransform.getPosition().y, arcPos.y);

    std::cout << "  ✓ Test 2 passed!" << std::endl;
    return true;
}

// Test 3: Wall rigidbody is static
bool test_WallRigidbodyIsStatic() {
    std::cout << "Running Test 3: Wall rigidbody is static..." << std::endl;

    flow::GameObject wall = WallGenerator::GenerateWall(sf::Vector2f(0, 0), 50, 0.0f, sf::Color::Green);

    auto* rb = wall.getComponent<flow::Rigidbody>();
    ASSERT_NE(rb, nullptr);

    // Verify the wall body is static
    b2BodyId bodyId = rb->getBodyId();
    b2BodyType bodyType = b2Body_GetType(bodyId);
    ASSERT_EQ(bodyType, b2_staticBody);

    std::cout << "  ✓ Test 3 passed!" << std::endl;
    return true;
}

// Test 4: Multiple walls in scene
bool test_MultipleWallsInScene() {
    std::cout << "Running Test 4: Multiple walls in scene..." << std::endl;

    auto scene = std::make_unique<flow::LevelScene>("wall_test_scene");

    // Add multiple walls to the scene
    for (int i = 0; i < 5; i++) {
        sf::Vector2f pos(i * 100.0f, i * 50.0f);
        auto wall = WallGenerator::GenerateWall(pos, 50, static_cast<float>(i * 10), sf::Color::White);
        scene->AddGameObject(std::move(wall));
    }

    std::cout << "  ✓ Test 4 passed!" << std::endl;
    return true;
}

// Test 5: Wall with different colors
bool test_WallWithDifferentColors() {
    std::cout << "Running Test 5: Walls with different colors..." << std::endl;

    sf::Color colors[] = {sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta};

    for (int i = 0; i < 5; i++) {
        flow::GameObject wall = WallGenerator::GenerateWall(
            sf::Vector2f(i * 100.0f, 0.0f),
            50,
            0.0f,
            colors[i]
        );
        ASSERT_NE(wall.getComponent<flow::Rigidbody>(), nullptr);
    }

    std::cout << "  ✓ Test 5 passed!" << std::endl;
    return true;
}

// Test 6: Wall with varying lengths
bool test_WallWithVaryingLengths() {
    std::cout << "Running Test 6: Walls with varying lengths..." << std::endl;

    int lengths[] = {10, 25, 50, 100, 200};

    for (int i = 0; i < 5; i++) {
        flow::GameObject wall = WallGenerator::GenerateWall(
            sf::Vector2f(0.0f, i * 100.0f),
            lengths[i],
            0.0f,
            sf::Color::White
        );
        ASSERT_NE(wall.getComponent<flow::Rigidbody>(), nullptr);
    }

    std::cout << "  ✓ Test 6 passed!" << std::endl;
    return true;
}

// Test 7: Arc wall with different subdivisions
bool test_ArcWallWithVaryingSubdivisions() {
    std::cout << "Running Test 7: Arc walls with varying subdivisions..." << std::endl;

    int subdivisions[] = {2, 4, 8, 16, 32};

    for (int i = 0; i < 5; i++) {
        flow::GameObject arcWall = WallGenerator::GenerateWall(
            sf::Vector2f(i * 150.0f, 0.0f),
            100,
            0.0f,
            1.57f,
            subdivisions[i],
            sf::Color::White
        );
        ASSERT_NE(arcWall.getComponent<flow::Rigidbody>(), nullptr);
    }

    std::cout << "  ✓ Test 7 passed!" << std::endl;
    return true;
}

// ===== Network Component Tests =====

// Test 8: NetworkEmitter component creation
bool test_NetworkEmitterCreation() {
    std::cout << "Running Test 8: NetworkEmitter component creation..." << std::endl;

    flow::GameObject networkObject;
    std::string eventName = "player_position_update";

    auto& emitter = networkObject.addComponent<flow::NetworkEmitter>(eventName);

    // Verify component was added
    auto* retrievedEmitter = networkObject.getComponent<flow::NetworkEmitter>();
    ASSERT_NE(retrievedEmitter, nullptr);
    ASSERT_EQ(&emitter, retrievedEmitter);

    std::cout << "  ✓ Test 8 passed!" << std::endl;
    return true;
}

// Test 9: NetworkGhost component creation
bool test_NetworkGhostCreation() {
    std::cout << "Running Test 9: NetworkGhost component creation..." << std::endl;

    flow::GameObject ghostObject;
    int ghostId = 42;

    auto& ghost = ghostObject.addComponent<flow::NetworkGhost>(ghostId);

    // Verify component was added
    auto* retrievedGhost = ghostObject.getComponent<flow::NetworkGhost>();
    ASSERT_NE(retrievedGhost, nullptr);
    ASSERT_EQ(&ghost, retrievedGhost);

    std::cout << "  ✓ Test 9 passed!" << std::endl;
    return true;
}

// Test 10: Multiple NetworkEmitters with different event names
bool test_MultipleNetworkEmitters() {
    std::cout << "Running Test 10: Multiple NetworkEmitters with different events..." << std::endl;

    flow::GameObject object;

    std::string eventNames[] = {
        "position_sync",
        "rotation_sync",
        "velocity_sync",
        "state_change"
    };

    for (int i = 0; i < 4; i++) {
        auto& emitter = object.addComponent<flow::NetworkEmitter>(eventNames[i]);
        ASSERT_NE(object.getComponent<flow::NetworkEmitter>(), nullptr);
    }

    std::cout << "  ✓ Test 10 passed!" << std::endl;
    return true;
}

// Test 11: Network ghost with rigidbody
bool test_NetworkGhostWithRigidbody() {
    std::cout << "Running Test 11: Network ghost with rigidbody..." << std::endl;

    flow::GameObject ghostPlayer;
    ghostPlayer.mTransform.setPosition(sf::Vector2f(250.0f, 150.0f));

    auto& rb = ghostPlayer.addComponent<flow::Rigidbody>();
    auto& ghost = ghostPlayer.addComponent<flow::NetworkGhost>(1);

    ASSERT_NE(ghostPlayer.getComponent<flow::Rigidbody>(), nullptr);
    ASSERT_NE(ghostPlayer.getComponent<flow::NetworkGhost>(), nullptr);

    std::cout << "  ✓ Test 11 passed!" << std::endl;
    return true;
}

// Test 12: Network ghost in scene
bool test_NetworkGhostInScene() {
    std::cout << "Running Test 12: Multiple network ghosts in scene..." << std::endl;

    auto scene = std::make_unique<flow::LevelScene>("network_ghost_scene");

    // Add multiple ghost players
    for (int i = 0; i < 4; i++) {
        flow::GameObject ghostPlayer;
        ghostPlayer.mTransform.setPosition(sf::Vector2f(100.0f + (i * 60.0f), 200.0f));

        auto& ghost = ghostPlayer.addComponent<flow::NetworkGhost>(i + 1);
        auto& rb = ghostPlayer.addComponent<flow::Rigidbody>();

        scene->AddGameObject(std::move(ghostPlayer));
    }

    std::cout << "  ✓ Test 12 passed!" << std::endl;
    return true;
}

// Test 13: Track with walls and network emitter
bool test_TrackWithNetworkComponents() {
    std::cout << "Running Test 13: Track with walls and network emitter..." << std::endl;

    auto scene = std::make_unique<flow::LevelScene>("network_track_scene");

    // Add walls to create track
    scene->AddGameObject(WallGenerator::GenerateWall(sf::Vector2f(50.0f, 200.0f), 400, 90.0f, sf::Color::Yellow));
    scene->AddGameObject(WallGenerator::GenerateWall(sf::Vector2f(350.0f, 200.0f), 400, 90.0f, sf::Color::Yellow));

    // Add arc section
    scene->AddGameObject(WallGenerator::GenerateWall(sf::Vector2f(200.0f, 100.0f), 100, 0.0f, 3.14f, 16, sf::Color::Cyan));

    // Add a networked object
    flow::GameObject networkObject;
    networkObject.mTransform.setPosition(sf::Vector2f(200.0f, 150.0f));
    auto& emitter = networkObject.addComponent<flow::NetworkEmitter>("player_position_sync");
    auto& rb = networkObject.addComponent<flow::Rigidbody>();

    scene->AddGameObject(std::move(networkObject));

    std::cout << "  ✓ Test 13 passed!" << std::endl;
    return true;
}

// Test 14: Multiplayer scenario with walls and ghosts
bool test_MultiplayerTrackWithGhosts() {
    std::cout << "Running Test 14: Multiplayer track with walls and ghosts..." << std::endl;

    auto scene = std::make_unique<flow::LevelScene>("multiplayer_track");

    // Create track layout
    sf::Color trackColor = sf::Color::White;
    scene->AddGameObject(WallGenerator::GenerateWall(sf::Vector2f(100.0f, 200.0f), 300, 0.0f, trackColor));
    scene->AddGameObject(WallGenerator::GenerateWall(sf::Vector2f(400.0f, 200.0f), 300, 0.0f, trackColor));

    // Add local player with network emitter
    flow::GameObject localPlayer;
    localPlayer.mTransform.setPosition(sf::Vector2f(250.0f, 200.0f));
    auto& localRb = localPlayer.addComponent<flow::Rigidbody>();
    auto& localEmitter = localPlayer.addComponent<flow::NetworkEmitter>("local_player_sync");
    scene->AddGameObject(std::move(localPlayer));

    // Add ghost players for remote players
    for (int i = 0; i < 3; i++) {
        flow::GameObject ghost;
        ghost.mTransform.setPosition(sf::Vector2f(250.0f + (i * 70.0f), 250.0f));
        auto& ghostRb = ghost.addComponent<flow::Rigidbody>();
        auto& ghostComp = ghost.addComponent<flow::NetworkGhost>(i + 1);
        scene->AddGameObject(std::move(ghost));
    }

    std::cout << "  ✓ Test 14 passed!" << std::endl;
    return true;
}

// Test 15: Complex track with multiple wall types and network setup
bool test_ComplexTrackWithNetworking() {
    std::cout << "Running Test 15: Complex track with multiple wall types and networking..." << std::endl;

    flow::PhysicsManager::getGlobal().setGravity(sf::Vector2f(0, 500.0f));

    auto scene = std::make_unique<flow::LevelScene>("complex_network_track");

    // Create left boundary wall
    scene->AddGameObject(WallGenerator::GenerateWall(sf::Vector2f(50.0f, 300.0f), 600, 90.0f, sf::Color::Red));

    // Create right boundary wall
    scene->AddGameObject(WallGenerator::GenerateWall(sf::Vector2f(450.0f, 300.0f), 600, 90.0f, sf::Color::Red));

    // Create curved sections
    scene->AddGameObject(WallGenerator::GenerateWall(sf::Vector2f(250.0f, 150.0f), 120, 0.0f, 3.14f, 20, sf::Color::Blue));
    scene->AddGameObject(WallGenerator::GenerateWall(sf::Vector2f(250.0f, 450.0f), 120, 3.14f, 3.14f, 20, sf::Color::Blue));

    // Add networked players
    for (int i = 0; i < 4; i++) {
        flow::GameObject player;
        player.mTransform.setPosition(sf::Vector2f(250.0f + (i * 50.0f), 250.0f));

        auto& rb = player.addComponent<flow::Rigidbody>();

        if (i == 0) {
            // Local player
            auto& emitter = player.addComponent<flow::NetworkEmitter>("local_player");
        } else {
            // Remote players as ghosts
            auto& ghost = player.addComponent<flow::NetworkGhost>(i);
        }

        scene->AddGameObject(std::move(player));
    }

    std::cout << "  ✓ Test 15 passed!" << std::endl;
    return true;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Advanced Game Engine Test Suite" << std::endl;
    std::cout << "   Walls, Network, and Multiplayer Tests" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    int passed = 0;
    int failed = 0;

    std::vector<std::pair<const char*, std::function<bool()>>> tests = {
        {"WallGenerator Linear Wall", test_WallGeneratorLinearWall},
        {"WallGenerator Arc Wall", test_WallGeneratorArcWall},
        {"Wall Rigidbody Is Static", test_WallRigidbodyIsStatic},
        {"Multiple Walls in Scene", test_MultipleWallsInScene},
        {"Walls with Different Colors", test_WallWithDifferentColors},
        {"Walls with Varying Lengths", test_WallWithVaryingLengths},
        {"Arc Walls with Varying Subdivisions", test_ArcWallWithVaryingSubdivisions},
        {"NetworkEmitter Creation", test_NetworkEmitterCreation},
        {"NetworkGhost Creation", test_NetworkGhostCreation},
        {"Multiple NetworkEmitters", test_MultipleNetworkEmitters},
        {"Network Ghost with Rigidbody", test_NetworkGhostWithRigidbody},
        {"Network Ghosts in Scene", test_NetworkGhostInScene},
        {"Track with Network Components", test_TrackWithNetworkComponents},
        {"Multiplayer Track with Ghosts", test_MultiplayerTrackWithGhosts},
        {"Complex Track with Networking", test_ComplexTrackWithNetworking}
    };

    for (size_t i = 0; i < tests.size(); i++) {
        try {
            if (tests[i].second()) {
                passed++;
            } else {
                std::cout << "  ✗ " << tests[i].first << " failed" << std::endl;
                failed++;
            }
        } catch (const std::exception& e) {
            std::cout << "  ✗ " << tests[i].first << " failed with exception: " << e.what() << std::endl;
            failed++;
        } catch (...) {
            std::cout << "  ✗ " << tests[i].first << " failed with unknown exception" << std::endl;
            failed++;
        }
    }

    std::cout << std::endl << "========================================" << std::endl;
    std::cout << "           Test Results Summary" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Passed: " << passed << "/" << tests.size() << std::endl;
    std::cout << "Failed: " << failed << "/" << tests.size() << std::endl;
    std::cout << "========================================" << std::endl;

    return (failed == 0) ? 0 : -1;
}
