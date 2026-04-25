#include "Player.hpp"

PlayerController::PlayerController() : _rb(nullptr), input(0){}

void PlayerController::init()
{
	_rb = mGameObject->getComponent<flow::Rigidbody>();
}

void PlayerController::fixedUpdate()
{
	//forces
	b2BodyId id = _rb->getBodyId();
	auto rot = b2Body_GetRotation(id);
	auto origin = b2Body_GetPosition(id);
	b2Vec2 localForce = { 0.0f, accel };
	auto worldForce = b2RotateVector(rot, localForce);


	//rotation time
	b2Body_ApplyTorque(id, input * rotSpeed, true);

	//Raycast thrust
	b2Vec2 ray = { 0.0f,-1.0f };
	ray = b2RotateVector(rot, ray);
	auto r = b2World_CastRayClosest(flow::PhysicsManager::getGlobal().getWorldId(), origin, ray, b2DefaultQueryFilter());
	b2Vec2 thrustForce = { 0.0f,0.0f };
	if (r.hit)
	{
		float rayDist = 1 - r.fraction;
		thrustForce = b2MulSV(rayDist, {0.f, nearObjAccel}); // scale force by distance
	}
	b2Body_ApplyForceToCenter(id, worldForce + thrustForce, true);
}
