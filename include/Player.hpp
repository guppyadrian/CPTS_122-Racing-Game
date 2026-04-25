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
	const float accel = 300;
	const float nearObjAccel = 300;
	const float rotSpeed = 1.f;
	void init();
	void update(float dt) {};
	void fixedUpdate();
	PlayerController();
	~PlayerController() = default;
};

