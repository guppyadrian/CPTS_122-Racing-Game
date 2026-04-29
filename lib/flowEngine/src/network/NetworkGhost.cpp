//
// Created by guppy on 4/25/26.
//

#include "flow/components/NetworkGhost.hpp"
#include "flow/PhysicsManager.hpp"
#include <iostream>

#include <box2d/box2d.h>

namespace flow
{
	void NetworkGhost::init()
	{
		_jetFlame = mGameObject->getComponent<flow::ParticleSystem>();
		if (_jetFlame == nullptr)
		{
			std::cerr << "Player failed to get Particle System Component" << std::endl;
		}
	}

	void NetworkGhost::update(float dt)
	{
		auto rot = b2MakeRot(mGameObject->mTransform.getRotationRad());
		auto origin =  b2Vec2(mGameObject->mTransform.getPosition().x, mGameObject->mTransform.getPosition().y);
		b2Vec2 ray1 = { 7.0f, 25.0f };
		b2Vec2 ray2 = { -7.0f, 25.0f };
		ray1 = b2RotateVector(rot, ray1);
		ray2 = b2RotateVector(rot, ray2);
		auto r1 = b2World_CastRayClosest(flow::PhysicsManager::getGlobal().getWorldId(), origin, ray1, b2DefaultQueryFilter());
		auto r2 = b2World_CastRayClosest(flow::PhysicsManager::getGlobal().getWorldId(), origin, ray2, b2DefaultQueryFilter());

		if (r1.hit || r2.hit)
		{
			_jetFlame->startEmit(); // start particle system
		}
		else
		{
			_jetFlame->stopEmit(); // stop particle system
		}
	}
}
