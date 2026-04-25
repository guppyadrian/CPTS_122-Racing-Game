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
	const float accel = 800;
	const float nearObjAccel = 800;
	const float rotSpeed = 70000.0f;
	const float rotBrakingPower = 70000.0f;
	void init();
	void update(float dt) {};
	void fixedUpdate();
	PlayerController();
	~PlayerController() = default;
};

