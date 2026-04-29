// Logan Rainchild
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
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
#include <flow/MusicManager.hpp>

#include "Player.hpp"
#include "LevelLoader.hpp"
#include "EndGoal.hpp"
#include "flow/NetworkManager.hpp"
#include "flow/components/NetworkEmitter.hpp"
#include "flow/components/NetworkGhostManager.hpp"
#include "network/NetworkManager.hpp"
#include "TrackClock.hpp"
#include "UI/LevelSelectScene.hpp"
#include "UI/MenuScene.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Game");
	window.setVerticalSyncEnabled(true);

	window.setFramerateLimit(480);

	flow::Renderer::getGlobalRenderer().attachWindow(&window);

	flow::PhysicsManager::getGlobal().setGravity(sf::Vector2f(0, 0.f));
	
	b2World_SetMaximumLinearSpeed(flow::PhysicsManager::getGlobal().getWorldId(), 500.f);
	
	// NETWORK
	gp::network::NetworkManager::Start();
	flow::NetworkManager::getGlobal().getClient().connect("10.59.233.190", 25550);
	
	flow::SceneManager::getGlobal().loadScene(std::make_unique<MenuScene>(window));
	flow::SceneManager::getGlobal().loadScene(std::make_unique<LevelSelectScene>(window));
	flow::SceneManager::getGlobal().switchScene("menu");

	sf::Font font;
	if (!font.openFromFile("assets/Pixel-Regular.ttf")) { // Load a font
		return -1; // Handle error
	}

	TrackClock trackClock(window, font);
	EndGoal::getInstance().trackClockRef = &trackClock;
	
	//LevelLoader load;
	//load.readFile("boost");
	EndGoal::getInstance().reset();

	sf::Clock dtClock;
	float dt;
	sf::Text fpsText(font);
	fpsText.setCharacterSize(30);
	fpsText.setFillColor(sf::Color::White);

	flow::audio::MusicManager::getGlobal().play();

	while (window.isOpen())
	{
		dt = dtClock.restart().asSeconds();

		flow::PhysicsManager::getGlobal().tick(dt);

		flow::SceneManager::getGlobal().update(dt);

		trackClock.update();
		EndGoal::getInstance().update();

		// simple fps logging
		float fps = 1.f / dt;
		fpsText.setString(std::to_string(static_cast<int>(fps)) + " FPS");

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) || sf::Joystick::isButtonPressed(0, 3) || EndGoal::getInstance().finished())
		{
			trackClock.reset();
			EndGoal::getInstance().reset();
		}

		flow::SceneManager::getGlobal().draw();
		if (auto* scene = dynamic_cast<flow::LevelScene*>(flow::SceneManager::getGlobal().getCurrentSceneptr())) // what am i doing with my life
		{
			window.setView(window.getDefaultView());
			window.draw(fpsText);
			window.draw(trackClock.getText());
			window.draw(trackClock.getFinalText());	
		}
		window.display();
	}

	//run save

	gp::network::NetworkManager::Stop();
}
