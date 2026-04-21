// PA9.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"
#include "SpriteRenderer.hpp"
#include "Renderer.hpp"
#include "Rigidbody.hpp"
#include "PhysicsManager.hpp"
#include "SceneManager.hpp"

int main()
{

	sf::RenderWindow window(sf::VideoMode({ 600, 400 }), "Game");

	flow::Renderer::getGlobalRenderer().attachWindow(&window);

	flow::PhysicsManager::get().setGravity(sf::Vector2f(0, 0));

	std::vector<std::unique_ptr<flow::GameObject>> gameobjects; // replace with scene management

	for (int i = 0; i < 20; i++)
	{
		// --- Create a new gameobject ---
		gameobjects.push_back(std::make_unique<flow::GameObject>()); // make a new GameObject

		// --- initialize the gameobjects transform ---
		gameobjects.back()->mTransform.setPosition(sf::Vector2f(i * 70, i * 30));
		gameobjects.back()->mTransform.setRotationDeg(0);
		gameobjects.back()->mTransform.setScale(sf::Vector2f(0.08f, 0.08f));

		// -- Create a sprite renderer component ---
		auto srComponent = std::make_unique<flow::SpriteRenderer>(std::string("assets/jonah.png")); // create a sprite renderer component
		gameobjects.back()->addComponent(std::move(srComponent)); // move the component into the object

		// Create a rigidbody component ---
		auto rbComponent = std::make_unique<flow::Rigidbody>(); // create a rigidbody

		// --- Configure the rigidBody's parameters --- 
		// Note: You can have multiple collision shapes on a single body!
		b2BodyId bodyId = rbComponent->getBodyId();
		b2Body_SetType(bodyId, b2_dynamicBody); // Make the body dynamic (it moves)
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = 0.1f;
		shapeDef.material.friction = 0.f;
		shapeDef.material.restitution = 0.f;

		// --- get the sprite (we added the SpriteRenderer just above) ---
		auto& sprite = gameobjects.back()->getComponent<flow::SpriteRenderer>()->getSprite();
		// --- local bounds = actual texture size in pixels ---
		sf::FloatRect local = sprite.getLocalBounds();

		// --- apply the GameObject transform scale ---
		sf::Vector2f scale = gameobjects.back()->mTransform.getScale();

		// --- Box2D box expects half-width and half-height ---
		sf::Vector2f halfExtents(local.size.x * scale.x * 0.5f, local.size.y * scale.y * 0.5f);
		std::cout << "Half extents: " << halfExtents.x << ", " << halfExtents.y << std::endl;
		b2Polygon box = b2MakeBox(halfExtents.x, halfExtents.y);

		// Attach it to the existing bodyId
		b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

		gameobjects.back()->addComponent(std::move(rbComponent)); // move the component into the object

		//Note: make sure you set up components before you move them into the class
		// or get a new reference after you move it because the old ptr will be null

	}

	// move to scene manager
	for (int i = 0; i < 20; i++)
	{
		gameobjects[i]->init();
	}

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

		// move to scene manager
		for (int i = 0; i < 20; i++)
		{
			gameobjects[i]->update(dt); // needs to recieve the real deltatime between frames
		}
		flow::PhysicsManager::get().tick(dt); // yes, this MUST recieve deltatime. It handles the fixed time steps using it

		// simple fps logging
		float fps = 1.f / dt;
		fpsText.setString(std::to_string(static_cast<int>(fps)) + " FPS");

		window.clear();
		flow::Renderer::getGlobalRenderer().drawAll();
		window.draw(fpsText);
		window.display();
	}


}
