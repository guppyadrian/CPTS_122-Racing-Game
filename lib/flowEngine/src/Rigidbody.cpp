#include "flow/components/Rigidbody.hpp"
#include "flow/PhysicsManager.hpp"
#include <iostream>

namespace flow
{
	Rigidbody::Rigidbody()
	{
		b2BodyDef def = b2DefaultBodyDef();
		mBodyId = PhysicsManager::getGlobal().addRigidbody(this, &def);
	}

	Rigidbody::~Rigidbody()
	{
		PhysicsManager::getGlobal().removeRigidbody(this);
		PhysicsManager::getGlobal().destroyBody(mBodyId);
	}

	void Rigidbody::init()
	{
		sf::Vector2f pos = mGameObject->mTransform.getPosition();
		float rot = mGameObject->mTransform.getRotationRad();

		b2Body_SetTransform(mBodyId, { pos.x, pos.y }, b2MakeRot(rot));
		std::cout << "initialized rb transform" << mBodyId.index1 << std::endl;
	}

	void Rigidbody::update(float dt)
	{

	}
	void Rigidbody::fixedUpdate()
	{

	}
}
