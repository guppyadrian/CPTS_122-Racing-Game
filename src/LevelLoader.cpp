#include "LevelLoader.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <utility>

#include <fstream>
#include <cstdio> //ITS BACK!

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
#include <flow/components/LookAheadCamera.hpp>
#include <flow/components/ParticleSystem.hpp>
#include <flow/MusicManager.hpp>
#include <flow/components/AudioListener.hpp>
#include <flow/components/AudioSource.hpp>

#include "EndGoal.hpp"
#include "Multiplayer.hpp"
#include "WallGenerator.hpp"
#include "Player.hpp"

#include "flow/components/NetworkEmitter.hpp"
#include "flow/components/NetworkGhost.hpp"
#include "flow/components/NetworkGhostManager.hpp"
#include "Panels.hpp"

std::unique_ptr<flow::LevelScene> LevelLoader::readFile(const std::string& fileUUID, const bool preview)
{
	//flow::Scene* curScene = flow::SceneManager::getGlobal().getCurrentSceneptr();
	//if ((curScene != nullptr) && (curScene->get_uuid() == fileUUID)) return std::unique_ptr<flow::LevelScene>(); // TODO: do I need to add this back?
	std::ifstream file("assets/levels/" + fileUUID + ".txt");
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + fileUUID);
	}

	_ss.str(""); //yes both "" and clear are needed; idk why
	_ss.clear();
	_ss << file.rdbuf();

	file.close();

	std::string line;

	std::string uuid;
	std::getline(_ss, uuid);

	int lvNum;
	std::getline(_ss, line); lvNum = std::stoi(line);

	sf::Color mainColor;
	int r = 0, g = 0, b = 0, a = 255;
	std::getline(_ss, line);
	sscanf(line.c_str(), "%d,%d,%d,%d", &r, &g, &b, &a);
	mainColor = sf::Color(r, g, b, a);

	std::string bgFile;
	std::getline(_ss, bgFile);

	std::string audioFile;
	std::getline(_ss, audioFile);

	sf::Vector2f playerPos;
	float playerRot;
	std::getline(_ss, line);
	sscanf(line.c_str(), "%f,%f,%f", &playerPos.x, &playerPos.y, &playerRot);

	float gravity;
	std::getline(_ss, line);
	gravity = std::stof(line);

	std::getline(_ss, line); //blank line
	//rest should be handled in _init() for objects

	return _init(gravity, fileUUID, bgFile, playerPos, playerRot, mainColor, lvNum, audioFile, preview);
}

std::unique_ptr<flow::LevelScene> LevelLoader::_init(const float& grav, const std::string& uuid, const std::string& bgFile,
	const sf::Vector2f& playerPos, const float& playerRot, const sf::Color& color, const int& lvNum, const std::string& audioFile, const bool preview)
{
	flow::PhysicsManager::getGlobal().setGravity(sf::Vector2f(0, grav));
	auto newScene = make_unique<flow::LevelScene>(uuid);

	flow::GameObject bg;
	bg.addComponent<flow::SpriteRenderer>(std::string("assets/bg/" + bgFile));
	newScene->AddGameObject(std::move(bg));

	flow::Rigidbody* pEndGoalObject = nullptr;

	if (!preview)
	{
		flow::audio::MusicManager::getGlobal().load(audioFile);
		flow::audio::MusicManager::getGlobal().setVolume(90.f);
	}

	flow::GameObject player;
	player.addComponent<flow::SpriteRenderer>(std::string("assets/player.png"));
	auto& rbComponent = player.addComponent<flow::Rigidbody>();

	//Object stuff
	std::string shapeLine;
	while (std::getline(_ss, shapeLine)) {
		if (shapeLine.empty()) continue;

		std::stringstream ss(shapeLine);
		std::string type;
		std::getline(ss, type, ';');

		if (type == "curve") {
			std::string posStr, radiusStr, angleStr, arcStr;
			std::getline(ss, posStr, ';');
			std::getline(ss, radiusStr, ';');
			std::getline(ss, angleStr, ';');
			std::getline(ss, arcStr, ';');

			float x, y;
			sscanf(posStr.c_str(), "%f,%f", &x, &y);
			float radius = std::stof(radiusStr);
			float angle = std::stof(angleStr) / 180;
			float arcLength = std::stof(arcStr) / 180;

			newScene->AddGameObject(WallGenerator::GenerateWall(
				{ x, y },
				radius,
				angle * B2_PI,
				arcLength * B2_PI,
				32,
				color
			));
		}
		else if (type == "line") {
			std::string posStr, lengthStr, angleStr;
			std::getline(ss, posStr, ';');
			std::getline(ss, lengthStr, ';');
			std::getline(ss, angleStr, ';');

			float x, y;
			sscanf(posStr.c_str(), "%f,%f", &x, &y);
			float length = std::stof(lengthStr);
			float angle = std::stof(angleStr);

			sf::Color c;
			if (_ss.eof())
			{
				c = { 255,255,0,255 };
			}
			else
			{
				c = color;
			}


			flow::GameObject lastWall = WallGenerator::GenerateWall({ x, y }, length, angle, c);
			// Tag its Box2D body as the end goal trigger
			pEndGoalObject = lastWall.getComponent<flow::Rigidbody>();
			if (pEndGoalObject) {
				b2Body_SetUserData(pEndGoalObject->getBodyId(), &EndGoal::getInstance());
			}

			newScene->AddGameObject(std::move(lastWall));
		}
		else if (type == "Boost")
		{
			std::string posStr, sizeStr, angleStr;
			std::getline(ss, posStr, ';');
			std::getline(ss, sizeStr, ';');
			std::getline(ss, angleStr, ';');

			float x, y;
			int w, h;
			sscanf(posStr.c_str(), "%f,%f", &x, &y);
			sscanf(sizeStr.c_str(), "%d,%d", &w, &h);

			float angle = std::stof(angleStr);
			flow::GameObject boosterPanel;
			auto& n = boosterPanel.addComponent<flow::SpriteRenderer>("assets/boost.png", sf::IntRect({ 0,0 }, { w,h }));
			boosterPanel.addComponent<BoosterPanel>(rbComponent.getBodyId(), *player.getComponent<flow::SpriteRenderer>(), n);
			boosterPanel.addComponent<flow::Rigidbody>(); //remove if Logan fixes Fixed Update :)
			boosterPanel.mTransform.setPosition({ x,y });
			boosterPanel.mTransform.setRotationDeg(angle);
			
			newScene->AddGameObject(std::move(boosterPanel));
		}
	}

	//Endgoal
	EndGoal& goal = EndGoal::getInstance();
	goal.setEndGoal(pEndGoalObject->getBodyId()); // TODO: this can be null according to clion -adrian
	goal.setLaps(lvNum);



	player.mTransform.setPosition(playerPos);
	player.mTransform.setRotationDeg(playerRot);
	player.mTransform.setScale(sf::Vector2f(0.02f, 0.02f));

	// narrow beam  particle system
	auto& ps2 = player.addComponent<flow::ParticleSystem>();
	ps2.setStartPosition({ 0.f, 200.f });
	ps2.setParticleCount(50);
	ps2.setStartRandomVelocity(1000.f);
	ps2.setStartVelocity({ 0.f, 30000.f });
	ps2.setStartColor(sf::Color(255, 200, 220, 128));
	ps2.setEndColor(sf::Color(255, 40, 200, 32));
	ps2.setStartSize(200);
	ps2.setEndSize(50);
	ps2.setStartLifetime(0.12f);

	// wider fire effect particle system
	auto& ps1 = player.addComponent<flow::ParticleSystem>();
	ps1.setStartPosition({ 0.f, 300.f });
	ps1.setParticleCount(500);
	ps1.setStartRandomVelocity(1500.f);
	ps1.setStartVelocity({ 0.f, 2000.f });
	ps1.setStartColor(sf::Color(250, 250, 250, 200));
	ps1.setEndColor(sf::Color(25, 100, 250, 150));
	ps1.setStartSize(50);
	ps1.setEndSize(25);
	ps1.setStartLifetime(0.3f);
	ps1.startEmit();

	// netowrk
	if (!preview && Multiplayer::getInstance().inMultiplayer)
	{
		player.addComponent<flow::NetworkEmitter>("plyr");
		player.addComponent<flow::NetworkGhostManager>("plyr", Multiplayer::getInstance().id);	
	}


	// --- Configure the rigidBody's parameters ---
	// Note: You can have multiple collision shapes on a single body!
	b2BodyId bodyId = rbComponent.getBodyId();
	b2Body_SetType(bodyId, b2_dynamicBody); // Make the body dynamic (it moves)
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 0.1f;
	shapeDef.material.friction = 0.f;
	shapeDef.material.restitution = 0.15f;

	// --- get the sprite (we added the SpriteRenderer just above) ---
	auto& sprite = player.getComponent<flow::SpriteRenderer>()->getSprite();
	// --- local bounds = actual texture size in pixels ---
	sf::FloatRect local = sprite.getLocalBounds();

	// --- apply the GameObject transform scale ---
	sf::Vector2f scale = player.mTransform.getScale();

	// --- Box2D box expects half-width and half-height ---
	float radius = std::min(local.size.x * scale.x, local.size.y * scale.y) * 0.5f;
	b2Circle circle = { {0.0f, 0.0f}, radius + 2.f };
	b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);
	b2Body_SetMassData(bodyId, { 11.f,b2Body_GetMassData(bodyId).center,150.f });

	//test for bullet?
	b2Body_SetBullet(player.getComponent<flow::Rigidbody>()->getBodyId(), true);

	//Add to endGoal
	goal.setPlayer(bodyId);
	goal.playerStartPos = playerPos;
	goal.playerStartRot = playerRot;

	player.addComponent<PlayerController>();
	player.getComponent<PlayerController>()->playerStartPos = playerPos;
	player.getComponent<PlayerController>()->playerStartRot = playerRot;

	sf::View view = sf::View({ 0,0 }, { 640, 384 });
	player.addComponent<flow::LookAheadCamera>(view);

	if (!preview)
	{
		player.addComponent<flow::audio::AudioListener>();

		auto& thrustAudio = player.addComponent<flow::audio::AudioSource>("assets/sfx/thrustLoop.mp3");
		thrustAudio.loop(true);
		thrustAudio.setVolume(67.f);
		thrustAudio.play();
	}

	newScene->AddGameObject(std::move(player));

	// load the scene
	return newScene;
}