#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include <iostream>
#include "EndGoal.hpp"

#include <flow/components/LookAheadCamera.hpp>

#include <flow/SceneManager.hpp>
#include <flow/MusicManager.hpp>
#include "UI/LevelSelectScene.hpp"
#include <algorithm>

#include "Multiplayer.hpp"
#include "flow/NetworkManager.hpp"
#include "UI/MenuScene.hpp"

void PlayerController::init()
{
	_rb = mGameObject->getComponent<flow::Rigidbody>();
	if (_rb == nullptr)
	{
		std::cerr << "Player failed to get Rigidbody Component" << std::endl;
	}
	_jetFlame = mGameObject->getComponent<flow::ParticleSystem>();
	if (_jetFlame == nullptr)
	{
		std::cerr << "Player failed to get Particle System Component" << std::endl;
	}
	_boostSFX = mGameObject->getComponent<flow::audio::AudioSource>();
	if (_boostSFX == nullptr)
	{
		std::cerr << "Player failed to get Particle System Component" << std::endl;
	}

	b2Body_SetLinearDamping(_rb->getBodyId(), 0.2f);
	b2Body_SetAngularDamping(_rb->getBodyId(), 0.3f);

	_startVolume = _boostSFX->getVolume();

	if (TrackClock::instance != nullptr)
	{
		TrackClock::instance->reset();
		EndGoal::getInstance().reset();
	}
}

void PlayerController::fixedUpdate()
{
	input = 0.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		input += -1.f; // Move Left
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		input += 1.f;  // Move Right
	}
	if (sf::Joystick::isConnected(0))
	{
		float x = 0;
			x += sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
			x += -1* sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);
		if (abs(x) > 0.01)
		{
			input += x / 100.f;
		}
	}

	//forces
	b2BodyId id = _rb->getBodyId();
	auto rot = b2Body_GetRotation(id);
	auto origin = b2Body_GetPosition(id);
	b2Vec2 localForce = { 0.0f, -accel };
	auto worldForce = b2RotateVector(rot, localForce);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) || sf::Joystick::isButtonPressed(0, 3) || EndGoal::getInstance().finished()) //3 is Y button, 9 is esc
	{
		reset();
	}

	// 1. Handle Rotation & Braking
	if (input != 0.f)
	{
		b2Body_ApplyTorque(id, input * rotSpeed, true);
	}
	else
	{
		// Get the current angular velocity
		float currentAngularVel = b2Body_GetAngularVelocity(id);

		// Calculate counter-torque (braking force)
		// Adjust 'brakingPower' to make the stop faster or slower
		float stopTorque = -currentAngularVel * rotBrakingPower;

		b2Body_ApplyTorque(id, stopTorque, true);
	}


	//Raycast thrust
	b2Vec2 thrustForce = { 0.0f,0.0f };
	b2Vec2 ray1 = { 7.0f, 25.0f };
	b2Vec2 ray2 = { -7.0f, 25.0f };
	ray1 = b2RotateVector(rot, ray1);
	ray2 = b2RotateVector(rot, ray2);
	auto r1 = b2World_CastRayClosest(flow::PhysicsManager::getGlobal().getWorldId(), origin, ray1, b2DefaultQueryFilter());
	auto r2 = b2World_CastRayClosest(flow::PhysicsManager::getGlobal().getWorldId(), origin, ray2, b2DefaultQueryFilter());


	if (r1.hit || r2.hit)
	{
		float rayDist = b2MaxFloat(((1 - r1.fraction) * (1 - r1.fraction)), ((1 - r2.fraction) * (1 - r2.fraction)));

		thrustForce = b2MulSV(rayDist, { 0.f, -nearObjAccel }); // scale force by distance
		thrustForce = b2RotateVector(rot, thrustForce);

		_jetFlame->startEmit(); // start particle system

		// boost sfx enable
		if (_boostSFX->getStatus() != sf::Music::Status::Playing)
		{
			_boostSFX->play();
			_fadeT = _audioFadeTime;
			_boostSFX->setVolume(_startVolume);
		}

		// catch faded audio and bring back too full
		if (_fadeT < _audioFadeTime)
		{
			_boostSFX->setVolume(_startVolume);
		}

		// don't let catch if half faded
		if (_fadeT < _audioFadeTime / 2.f)
		{
			_boostSFX->stop();
		}
	}
	else
	{
		_jetFlame->stopEmit(); // stop particle system

		// fade out audio
		if (_boostSFX->getStatus() != sf::Music::Status::Stopped && _fadeT <= 0)
		{
			_boostSFX->stop();
		}
		_fadeT -= flow::PhysicsManager::getFixedTimestep();
		_boostSFX->setVolume(std::max(_startVolume * (_fadeT / _audioFadeTime), 0.f));
	}
	b2Body_ApplyForceToCenter(id, worldForce + thrustForce, true);
	
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		if (Multiplayer::getInstance().inMultiplayer)
		{
			//flow::SceneManager::getGlobal().loadScene(std::make_unique<MenuScene>(flow::Renderer::getGlobalRenderer().getWindow()));
			//flow::SceneManager::getGlobal().switchScene("multiplayer-lobby");
			if (!Multiplayer::getInstance().server)
			{
				flow::SceneManager::getGlobal().loadScene(std::make_unique<MenuScene>(flow::Renderer::getGlobalRenderer().getWindow()));
				flow::SceneManager::getGlobal().switchScene("menu");
			} else
			if (!Multiplayer::getInstance().endEmitted)
			{
				flow::NetworkManager::getGlobal().getServer().emit("end-game", '\0');
				Multiplayer::getInstance().endEmitted = true;
			}
		}
		else // not in multiplayer
		{
			flow::SceneManager::getGlobal().loadScene(std::make_unique<LevelSelectScene>(flow::Renderer::getGlobalRenderer().getWindow()));
			flow::SceneManager::getGlobal().switchScene("level-select"); // adrian: dude why does this have to be 2 lines???!
		}
		
		flow::audio::MusicManager::getGlobal().stop();
	}
}


void PlayerController::reset()
{
	b2BodyId id = _rb->getBodyId();

	float radians = playerStartRot * (B2_PI / 180.0f);

	b2Rot myRotation;
	myRotation.c = b2ComputeCosSin(radians).cosine;
	myRotation.s = b2ComputeCosSin(radians).sine;
	b2Body_SetTransform(id, { playerStartPos.x , playerStartPos.y }, myRotation);
	b2Body_SetAngularVelocity(id, 0.f);
	b2Body_SetLinearVelocity(id, { 0.f,0.f });
	mGameObject->getComponent<flow::LookAheadCamera>()->reset();

	if (TrackClock::instance != nullptr)
	{
		TrackClock::instance->reset();
	}

	EndGoal::getInstance().reset();
}
