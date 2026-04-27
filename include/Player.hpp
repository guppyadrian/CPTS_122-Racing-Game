#pragma once
#include <flow/Component.hpp>
#include <flow/GameObject.hpp>
#include <flow/components/Rigidbody.hpp>
#include <flow/PhysicsManager.hpp>
#include <flow/components/ParticleSystem.hpp>

class PlayerController : public flow::Component
{
private:
	flow::Rigidbody* _rb;
public:
	float input; //left is negative, right is positive
	const float accel = 1200.f; //1000
	const float nearObjAccel = 2500.f; //2000
	const float rotSpeed = 3000.0f; //8k
	const float rotBrakingPower = 1000.0f; //2k
	sf::Vector2f playerStartPos;
	float playerStartRot;

	flow::ParticleSystem* _jetFlame;

	void init();
	void update(float dt) {};
	void fixedUpdate();
	PlayerController();
	~PlayerController() = default;
};

