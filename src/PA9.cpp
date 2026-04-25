// Logan Rainchild

#include <iostream>
#include <memory>
#include <vector>
#include <utility>

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


int main()
{

	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Game");

	window.setFramerateLimit(240);

	flow::Renderer::getGlobalRenderer().attachWindow(&window);

	flow::PhysicsManager::getGlobal().setGravity(sf::Vector2f(0, 0));

	auto newScene = make_unique<flow::LevelScene>(std::string("my scene"));


	//walls and stuff

	//flow::GameObject straightWall = WallGenerator::GenerateWall({100,100}, 100, 0, sf::Color::Red);
	//newScene->AddGameObject(std::move(straightWall));

	// example when you dont need to use std::move
	newScene->AddGameObject(WallGenerator::GenerateWall({150, 150}, 150, 1.57f, -3.141f, 600, sf::Color::Red));
	newScene->AddGameObject(WallGenerator::GenerateWall({ -150, 150 }, 150, -1.57f, -3.141f, 600, sf::Color::Red));

	newScene->AddGameObject(WallGenerator::GenerateWall({ 0,0 }, 300, 0, sf::Color::White));
	newScene->AddGameObject(WallGenerator::GenerateWall({ 0,300 }, 300, 0, sf::Color::White));


	flow::GameObject player = flow::GameObject();

	player.mTransform.setPosition(sf::Vector2f(120,180));
	player.mTransform.setRotationDeg(0);
	player.mTransform.setScale(sf::Vector2f(0.02f, 0.02f));

	player.addComponent<flow::SpriteRenderer>(std::string("assets/player.png"));

	auto& rbComponent = player.addComponent<flow::Rigidbody>();

	// --- Configure the rigidBody's parameters ---
	// Note: You can have multiple collision shapes on a single body!
	b2BodyId bodyId = rbComponent.getBodyId();
	b2Body_SetType(bodyId, b2_dynamicBody); // Make the body dynamic (it moves)
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 0.1f;
	shapeDef.material.friction = 0.f;
	shapeDef.material.restitution = 0.3f;

	// --- get the sprite (we added the SpriteRenderer just above) ---
	auto& sprite = player.getComponent<flow::SpriteRenderer>()->getSprite();
	// --- local bounds = actual texture size in pixels ---
	sf::FloatRect local = sprite.getLocalBounds();

	// --- apply the GameObject transform scale ---
	sf::Vector2f scale = player.mTransform.getScale();

	// --- Box2D box expects half-width and half-height ---
	sf::Vector2f halfExtents(local.size.x * scale.x * 0.5f, local.size.y * scale.y * 0.5f);
	std::cout << "Half extents: " << halfExtents.x << ", " << halfExtents.y << std::endl;
	b2Polygon box = b2MakeBox(halfExtents.x, halfExtents.y);

	// Attach it to the existing bodyId
	b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

	player.addComponent<PlayerController>();

	sf::View view = sf::View({ 0,0 }, { 600, 400 });
	player.addComponent<flow::Camera>(view);

	newScene->AddGameObject(std::move(player));


	// load the scene
	flow::SceneManager::getGlobal().loadScene(std::move(newScene));
	flow::SceneManager::getGlobal().switchScene("my scene");

	sf::Font font;
	if (!font.openFromFile("assets/Pixel-Regular.ttf")) { // Load a font
		return -1; // Handle error
	}

	sf::Clock dtClock;
	float dt;

	sf::Text fpsText(font);
	fpsText.setCharacterSize(30);
	fpsText.setFillColor(sf::Color::White);

	while (window.isOpen())
	{
		dt = dtClock.restart().asSeconds();

		// SFML in this workspace uses an optional-style pollEvent that returns
		// std::optional<sf::Event>. Use that form to handle events.
		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		flow::SceneManager::getGlobal().update(dt);

		flow::PhysicsManager::getGlobal().tick(dt);

		// simple fps logging
		float fps = 1.f / dt;
		fpsText.setString(std::to_string(static_cast<int>(fps)) + " FPS");

		window.clear();
		flow::Renderer::getGlobalRenderer().drawAll();
		window.setView(window.getDefaultView());
		window.draw(fpsText);
		window.display();
	}


}
