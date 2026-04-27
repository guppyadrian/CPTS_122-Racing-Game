// Lane Evanger

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

#include "MainMenu.hpp"
#include "WallGenerator.hpp"


int main()
{
	sf::RenderWindow window(sf::VideoMode({ 600, 400 }), "Game");

	window.setFramerateLimit(60);

	flow::Renderer::getGlobalRenderer().attachWindow(&window);

	flow::PhysicsManager::getGlobal().setGravity(sf::Vector2f(0, 0));

	auto mainMenu = std::make_unique<MainMenu>();

	flow::SceneManager::getGlobal().loadScene(std::move(mainMenu));
	flow::SceneManager::getGlobal().switchScene("main-menu");

	sf::Font font;
	if (!font.openFromFile("assets/Pixel-Regular.ttf")) {
		return -1;
	}

	sf::Clock dtClock;
	float dt;

	sf::Text fpsText(font);
	fpsText.setCharacterSize(30);
	fpsText.setFillColor(sf::Color::White);

	while (window.isOpen())
	{
		window.clear();
		dt = dtClock.restart().asSeconds();

		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		flow::SceneManager::getGlobal().update(dt);

		flow::PhysicsManager::getGlobal().tick(dt);

		float fps = 1.f / dt;
		fpsText.setString(std::to_string(static_cast<int>(fps)) + " FPS");


		flow::Renderer::getGlobalRenderer().drawAll();
		window.draw(fpsText);
		window.display();
	}
}