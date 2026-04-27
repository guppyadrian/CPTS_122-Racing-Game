#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include <iostream>
#include "EndGoal.hpp"

PlayerController::PlayerController() : _rb(nullptr), input(0) {}

void PlayerController::init()
{
	_rb = mGameObject->getComponent<flow::Rigidbody>();
	if (_rb == nullptr)
	{
		int i = 0;
		i++;
	}
	b2Body_SetLinearDamping(_rb->getBodyId(), 0.4f);
	b2Body_SetAngularDamping(_rb->getBodyId(), 0.3f);
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

	EndGoal& endGoal = EndGoal::getInstance();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) || sf::Joystick::isButtonPressed(0, 3) || endGoal.isPlayerReachedEnd()) //3 is Y button, 9 is esc
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
	b2Vec2 ray1 = { 5.0f, 15.0f };
	b2Vec2 ray2 = { -5.0f, 15.0f };
	ray1 = b2RotateVector(rot, ray1);
	ray2 = b2RotateVector(rot, ray2);
	auto r1 = b2World_CastRayClosest(flow::PhysicsManager::getGlobal().getWorldId(), origin, ray1, b2DefaultQueryFilter());
	auto r2 = b2World_CastRayClosest(flow::PhysicsManager::getGlobal().getWorldId(), origin, ray2, b2DefaultQueryFilter());

	if (r1.hit || r2.hit)
	{
		float rayDist = b2MaxFloat(((1 - r1.fraction)* (1 - r1.fraction)), ((1 - r2.fraction)* (1 - r2.fraction)));

		thrustForce = b2MulSV(rayDist, { 0.f, -nearObjAccel }); // scale force by distance
		thrustForce = b2RotateVector(rot, thrustForce);
	}
	b2Body_ApplyForceToCenter(id, worldForce + thrustForce, true);
}
