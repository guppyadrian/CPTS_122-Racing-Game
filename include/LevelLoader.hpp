#pragma once //Include everything bc I said so
#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#include <cstdio> //ITS BACK!

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>

#include <flow/Component.hpp>
#include <flow/GameObject.hpp>
#include <flow/components/SpriteRenderer.hpp>
#include <flow/Renderer.hpp>
#include <flow/components/Rigidbody.hpp>
#include <flow/PhysicsManager.hpp>
#include <flow/SceneManager.hpp>
#include <flow/LevelScene.hpp>
#include <flow/components/Camera.hpp>

#include "WallGenerator.hpp"
#include "Player.hpp"

class LevelLoader
{
private:
	std::stringstream _ss;
	void _init(const float& grav, const std::string& uuid, const std::string& bgFile,
		const sf::Vector2f& playerPos, const float& playerRot);
public:
	LevelLoader() = default;
	~LevelLoader() = default;
	void readFile(std::string uuid);
	void onExit(); //ie. reset everything to default
};

void LevelLoader::_init(const float& grav, const std::string& uuid, const std::string& bgFile,
	const sf::Vector2f& playerPos, const float& playerRot)
{
	flow::PhysicsManager::getGlobal().setGravity(sf::Vector2f(0, grav));
	auto newScene = make_unique<flow::LevelScene>(uuid);

	flow::GameObject bg = flow::GameObject();
	bg.addComponent<flow::SpriteRenderer>(std::string("assets/" + bgFile));
	newScene->AddGameObject(std::move(bg));

	//Object stuff


	//Player stuff
	flow::GameObject player = flow::GameObject();
	player.mTransform.setPosition(playerPos);
	player.mTransform.setRotationDeg(playerRot);
	player.mTransform.setScale(sf::Vector2f(0.02f, 0.02f));
	player.addComponent<flow::SpriteRenderer>(std::string("assets/player.png"));
	auto& rbComponent = player.addComponent<flow::Rigidbody>();
	b2BodyId bodyId = rbComponent.getBodyId();
	b2Body_SetType(bodyId, b2_dynamicBody); // Make the body dynamic (it moves)
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 0.1f;
	shapeDef.material.friction = 0.f;
	shapeDef.material.restitution = 0.25f;
	auto& sprite = player.getComponent<flow::SpriteRenderer>()->getSprite();
	sf::FloatRect local = sprite.getLocalBounds();
	sf::Vector2f scale = player.mTransform.getScale();
	sf::Vector2f halfExtents(local.size.x * scale.x * 0.5f, local.size.y * scale.y * 0.5f);
	std::cout << "Half extents: " << halfExtents.x << ", " << halfExtents.y << std::endl;
	b2Polygon box = b2MakeBox(halfExtents.x, halfExtents.y);
	b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

	player.addComponent<PlayerController>();
	sf::View view = sf::View({ 0,0 }, { 600, 400 });
	player.addComponent<flow::Camera>(view);
	newScene->AddGameObject(std::move(player));

	flow::SceneManager::getGlobal().loadScene(std::move(newScene));
	flow::SceneManager::getGlobal().switchScene(uuid);
	return;
}