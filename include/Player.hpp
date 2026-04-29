/**
 * @file Player.hpp
 * @author Johnathan Emanuelli
 * @author Logan Rainchild
 * @brief Player Controller Component that handles player physics and FX
 */

#pragma once
#include <flow/Component.hpp>
#include <flow/GameObject.hpp>
#include <flow/components/Rigidbody.hpp>
#include <flow/PhysicsManager.hpp>
#include <flow/components/ParticleSystem.hpp>
#include <flow/components/AudioSource.hpp>

class PlayerController : public flow::Component
{
private:
	flow::Rigidbody* _rb{};
	flow::audio::AudioSource* _boostSFX{};
	const float _audioFadeTime = 0.75f;
	float _fadeT = 0;
	float _startVolume;
public:
	float input{}; //left is negative, right is positive
	const float accel = 800.f; //1000
	const float nearObjAccel = 800.f; //2000
	const float rotSpeed = 3000.0f; //8k
	const float rotBrakingPower = 1000.0f; //2k
	sf::Vector2f playerStartPos{};
	float playerStartRot{};

	flow::ParticleSystem* _jetFlame{};

	void init() override;
	void update(float dt) override {};
	void fixedUpdate() override;
	PlayerController() = default;
	~PlayerController() override = default;
};

