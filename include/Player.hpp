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
	const float accel = 3;
	const float nearObjAccel = 3;
	const float rotSpeed = 0.1f;
	void init();
	void update(float dt) {};
	void fixedUpdate();
	PlayerController();
	~PlayerController() = default;
};

