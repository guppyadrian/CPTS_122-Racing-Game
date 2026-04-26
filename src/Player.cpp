#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
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
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		input = -1.f; // Move Left
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		input = 1.f;  // Move Right
	}

	//forces
	b2BodyId id = _rb->getBodyId();
	auto rot = b2Body_GetRotation(id);
	auto origin = b2Body_GetPosition(id);
	b2Vec2 localForce = { 0.0f, -accel };
	auto worldForce = b2RotateVector(rot, localForce);

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
