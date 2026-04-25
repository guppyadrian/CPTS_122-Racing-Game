#pragma once
#include <flow/Component.hpp>
#include <flow/GameObject.hpp>
#include <flow/components/Rigidbody.hpp>
#include <flow/PhysicsManager.hpp>

class PlayerController : public flow::Component
{
private:
	flow::Rigidbody* _rb;
public:
	float input; //left is negative, right is positive
	const float accel = 1000.f; //1000
	const float nearObjAccel = 6000.f; //2000
	const float rotSpeed = 8000.0f;
	const float rotBrakingPower = 2000.0f;
	void init();
	void update(float dt) {};
	void fixedUpdate();
	PlayerController();
	~PlayerController() = default;
};

