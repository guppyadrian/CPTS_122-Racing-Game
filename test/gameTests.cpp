//
// Created by guppy on 5/1/26.
//

#include <catch2/catch_test_macros.hpp>

#include "flow/GameObject.hpp"
#include "flow/components/Rigidbody.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "WallGenerator.hpp"
#include "flow/LevelScene.hpp"
#include "flow/PhysicsManager.hpp"
#include "flow/components/NetworkEmitter.hpp"
#include "flow/components/NetworkGhost.hpp"
#include "UI/ButtonArray.hpp"

TEST_CASE("Wall Generator: Line", "[game][gameObject][wallGenerator]")
{
    sf::Vector2f wallPos(200.0f, 300.0f);
    int wallLength = 50;
    float wallAngle = 45.0f;
    sf::Color wallColor = sf::Color::Red;

    flow::GameObject wall = WallGenerator::GenerateWall(wallPos, wallLength, wallAngle, wallColor);

    // Verify wall was created with correct properties
    REQUIRE(wall.getComponent<flow::Rigidbody>() != nullptr);
    REQUIRE(wall.mTransform.getPosition().x == wallPos.x);
    REQUIRE(wall.mTransform.getPosition().y == wallPos.y);
    REQUIRE(wall.mTransform.getRotationDeg() == wallAngle);
}

TEST_CASE( "Wall Generator: Curve", "[game][gameObject][wallGenerator]" )
{
    sf::Vector2f arcPos(150.0f, 250.0f);
    int radius = 100;
    float startAngle = 0.0f;
    float arcLength = 1.57f; // 90 degrees in radians
    int subdivisions = 8;
    sf::Color arcColor = sf::Color::Blue;

    flow::GameObject arcWall = WallGenerator::GenerateWall(arcPos, radius, startAngle, arcLength, subdivisions, arcColor);

    // Verify arc wall was created with correct properties
    REQUIRE(arcWall.getComponent<flow::Rigidbody>() != nullptr);
    REQUIRE(arcWall.mTransform.getPosition().x == arcPos.x);
    REQUIRE(arcWall.mTransform.getPosition().y == arcPos.y);
}

TEST_CASE( "Wall Rigidbody is Static", "[game][gameObject][wallGenerator][rigidbody]" )
{
    flow::GameObject wall = WallGenerator::GenerateWall(sf::Vector2f(0, 0), 50, 0.0f, sf::Color::Green);

    auto* rb = wall.getComponent<flow::Rigidbody>();
    REQUIRE(rb != nullptr);

    // Verify the wall body is static
    b2BodyId bodyId = rb->getBodyId();
    b2BodyType bodyType = b2Body_GetType(bodyId);
    REQUIRE(bodyType == b2_staticBody);
}

TEST_CASE( "Multiple Walls", "[game][gameObject][wallGenerator]" )
{
    auto scene = std::make_unique<flow::LevelScene>("wall_test_scene");

    // Add multiple walls to the scene
    for (int i = 0; i < 5; i++) {
        sf::Vector2f pos(i * 100.0f, i * 50.0f);
        auto wall = WallGenerator::GenerateWall(pos, 50, static_cast<float>(i * 10), sf::Color::White);
        scene->AddGameObject(std::move(wall));
    }
}

TEST_CASE( "Wall Colors", "[game][gameObject][wallGenerator]" )
{
    sf::Color colors[] = {sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta};

    for (int i = 0; i < 5; i++) {
        flow::GameObject wall = WallGenerator::GenerateWall(
            sf::Vector2f(i * 100.0f, 0.0f),
            50,
            0.0f,
            colors[i]
        );
        REQUIRE(wall.getComponent<flow::Rigidbody>() != nullptr);
    }
}

TEST_CASE( "Wall Lengths", "[game][gameObject][wallGenerator]" )
{
    int lengths[] = {10, 25, 50, 100, 200};

    for (int i = 0; i < 5; i++) {
        flow::GameObject wall = WallGenerator::GenerateWall(
            sf::Vector2f(0.0f, i * 100.0f),
            lengths[i],
            0.0f,
            sf::Color::White
        );
        REQUIRE(wall.getComponent<flow::Rigidbody>() != nullptr);
    }
}

TEST_CASE( "Arc Walls Different Subdivisions", "[game][gameObject][wallGenerator]" )
{
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
        REQUIRE(arcWall.getComponent<flow::Rigidbody>() != nullptr);
    }
}

TEST_CASE( "Track with Network", "[game][network][networkEmitter][scene]" )
{
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
}

TEST_CASE( "Multiplayer Track", "[game][network][networkGhost][networkEmitter][scene]" )
{
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
}

TEST_CASE( "Complex Track Multiplayer", "[game][network][networkGhost][networkEmitter][scene]" )
{
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
}

TEST_CASE( "ButtonArray", "[game][ui]" )
{
    ButtonArray buttonArray;
    
    buttonArray.add("player.png", {0, 0});
    buttonArray.add("player.png", {0, 0});
    buttonArray.add("player.png", {0, 0});
    REQUIRE(buttonArray.selected() == 0);
    
    buttonArray.next();
    buttonArray.next();
    REQUIRE(buttonArray.selected() == 2);
    
    buttonArray.next();
    REQUIRE(buttonArray.selected() == 2);
    
    buttonArray.prev();
    REQUIRE(buttonArray.selected() == 1);
    
    buttonArray.prev();
    buttonArray.prev();
    REQUIRE(buttonArray.selected() == 0);
}