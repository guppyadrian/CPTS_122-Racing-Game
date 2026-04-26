#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include <iostream>

PlayerController::PlayerController() : _rb(nullptr), input(0) {}

void PlayerController::init()
{
	_rb = mGameObject->getComponent<flow::Rigidbody>();
	if (_rb == nullptr)
	{
		int i = 0;
		i++;
	}
}

void PlayerController::fixedUpdate()
{
	input = 0.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
		input += -1.f; // Move Left
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		input += 1.f;  // Move Right
	}
	if (sf::Joystick::isConnected(0))
	{
		float x = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		float radians = playerStartRot * (B2_PI / 180.0f);

		b2Rot myRotation;
		myRotation.c = b2ComputeCosSin(radians).cosine;
		myRotation.s = b2ComputeCosSin(radians).sine;
		b2Body_SetTransform(id, { playerStartPos.x , playerStartPos.y }, myRotation);
		b2Body_SetAngularVelocity(id, 0.f);
		b2Body_SetLinearVelocity(id, { 0.f,0.f });
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
	b2Vec2 ray = { 0.0f, 20.0f };
	ray = b2RotateVector(rot, ray);
	auto r = b2World_CastRayClosest(flow::PhysicsManager::getGlobal().getWorldId(), origin, ray, b2DefaultQueryFilter());

	if (r.hit)
	{
		float rayDist = 1 - r.fraction;
		thrustForce = b2MulSV(rayDist, { 0.f, -nearObjAccel }); // scale force by distance
		thrustForce = b2RotateVector(rot, thrustForce);
	}

	b2Body_ApplyForceToCenter(id, worldForce + thrustForce, true);
}
