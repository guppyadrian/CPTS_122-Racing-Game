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

int main()
{

	sf::RenderWindow window(sf::VideoMode({ 600, 400 }), "Game");

	flow::Renderer::getGlobalRenderer().attachWindow(&window);

	std::vector<std::unique_ptr<flow::GameObject>> gameobjects; // replace with scene management

	for (int i = 0; i < 20; i++)
	{
		// --- Create a new gameobject ---
		gameobjects.push_back(std::make_unique<flow::GameObject>()); // make a new GameObject

		// --- initialize its transform ---
		gameobjects.back()->mTransform.setPosition(sf::Vector2f(i * 50, i * 50));
		gameobjects.back()->mTransform.setRotationDeg(0);
		gameobjects.back()->mTransform.setScale(sf::Vector2f(0.1f, 0.1f));

		// -- Create a sprite renderer component ---
		auto srComponent = std::make_unique<flow::SpriteRenderer>(std::string("assets/jonah.png")); // create a sprite renderer component
		gameobjects.back()->addComponent(std::move(srComponent)); // move the component into the object

		// Create a rigidbody component ---
		auto rbComponent = std::make_unique<flow::Rigidbody>(); // create a rigidbody
		gameobjects.back()->addComponent(std::move(rbComponent)); // move the component into the object

		// --- Create the shape for the rigidBody --- 
		// Note: You can have multiple collision shapes on a single body!
		b2BodyId bodyId = rbComponent->getBodyId();
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.material.friction = 0.f;
		shapeDef.material.restitution = 0.f;

		// Create the box geometry (half-width, half-height)
		b2Polygon box = b2MakeBox(1.0f, 1.0f);

		// Attach it to the existing bodyId
		b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

	}

	// move to scene manager
	for (int i = 0; i < 20; i++)
	{
		gameobjects[i]->init();
	}

	while (window.isOpen())
	{
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
			gameobjects[i]->update(20); // needs to recieve the real deltatime between frames
		}

		window.clear();
		flow::Renderer::getGlobalRenderer().drawAll();
		window.display();
	}


}
