/**
 * @file Pannels.hpp
 * @author Johnathan Emanuelli
 * @brief A component for boost pads
 */

#pragma once
#include <flow/Component.hpp>
#include <flow/GameObject.hpp>
#include <flow/components/SpriteRenderer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window.hpp>
#include "Player.hpp"

class BoosterPanel : public flow::Component
{
public:
	BoosterPanel(b2BodyId player, flow::SpriteRenderer& playerSprite, flow::SpriteRenderer& boostSprite) : _player(player), _playerSprite(playerSprite), _boostSprite(boostSprite)
	{
	}
	~BoosterPanel() = default;

	void init() override
	{
		//_boostSprite = mGameObject->getComponent<flow::SpriteRenderer>();
	}
	void update(float dt) override
	{

	}
	void fixedUpdate() override
	{
		sf::FloatRect boundsA = _boostSprite.getSprite().getLocalBounds();
		boundsA.size = boundsA.size.componentWiseMul(mGameObject->mTransform.getScale());
		boundsA.position = mGameObject->mTransform.getPosition() - boundsA.size.componentWiseMul({0.5f, 0.5f});

		sf::FloatRect boundsB = _playerSprite.getSprite().getLocalBounds();
		boundsB.size = boundsB.size.componentWiseMul(_playerSprite.mGameObject->mTransform.getScale());
		boundsB.position = _playerSprite.mGameObject->mTransform.getPosition() - boundsB.size.componentWiseMul({ 0.5f, 0.5f });


		if (boundsA.findIntersection(boundsB))
		{
			auto rot = b2Body_GetRotation(_player);
			auto f = b2RotateVector(rot, { 0.f, -_force });
			b2Body_ApplyForceToCenter(_player, f, true);
		}
	}
	float getForce() { return _force; }
	void setForce(float force) { _force = force; }

private:
	b2BodyId _player;
	flow::SpriteRenderer& _playerSprite;
	flow::SpriteRenderer& _boostSprite;
	float _force = 4000.f;
};