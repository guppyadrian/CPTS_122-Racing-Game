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

#include "Player.hpp"
#include "LevelLoader.hpp"
#include "EndGoal.hpp"
#include "flow/NetworkManager.hpp"
#include "flow/components/NetworkEmitter.hpp"
#include "flow/components/NetworkGhostManager.hpp"
#include "network/NetworkManager.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Game");
	window.setVerticalSyncEnabled(true);

	window.setFramerateLimit(480);

	flow::Renderer::getGlobalRenderer().attachWindow(&window);

	flow::PhysicsManager::getGlobal().setGravity(sf::Vector2f(0, 0.f));
	
	b2World_SetMaximumLinearSpeed(flow::PhysicsManager::getGlobal().getWorldId(), 200.f);
	
	// NETWORK
	gp::network::NetworkManager::Start();
	flow::NetworkManager::getGlobal().getClient().connect("10.59.226.61", 25550);
	
	LevelLoader load;
	load.readFile("Triangle");

	sf::Font font;
	if (!font.openFromFile("assets/Pixel-Regular.ttf")) { // Load a font
		return -1; // Handle error
	}

	
	sf::Clock trackClock;
	sf::Text trackText(font);
	trackText.setCharacterSize(60);
	trackText.setFillColor(sf::Color::White);
	trackText.setPosition({ window.getSize().x - 300.f, 0 });

	sf::Clock dtClock;
	float dt;
	sf::Text fpsText(font);
	fpsText.setCharacterSize(30);
	fpsText.setFillColor(sf::Color::White);

	while (window.isOpen())
	{
		dt = dtClock.restart().asSeconds();
		EndGoal::getInstance().update();
		// SFML in this workspace uses an optional-style pollEvent that returns
		// std::optional<sf::Event>. Use that form to handle events.
		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		flow::PhysicsManager::getGlobal().tick(dt);

		flow::SceneManager::getGlobal().update(dt);

		// simple fps logging
		float fps = 1.f / dt;
		fpsText.setString(std::to_string(static_cast<int>(fps)) + " FPS");

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) || sf::Joystick::isButtonPressed(0, 3) || EndGoal::getInstance().getCollide())
		{
			EndGoal::getInstance().setCollide(false);
			trackClock.restart();
		}
		int tSec = trackClock.getElapsedTime().asMilliseconds() / 1000;
		int tMs = trackClock.getElapsedTime().asMilliseconds() % 1000;
		trackText.setString(std::to_string(tSec) + ":" + std::to_string(tMs));

		window.clear();
		flow::Renderer::getGlobalRenderer().drawAll();
		window.setView(window.getDefaultView());
		window.draw(fpsText);
		window.draw(trackText);
		window.display();
	}

	gp::network::NetworkManager::Stop();
}
