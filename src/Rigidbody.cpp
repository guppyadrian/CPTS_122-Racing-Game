#include "Rigidbody.hpp"
#include "PhysicsManager.hpp"
#include <iostream>

namespace flow
{
	Rigidbody::Rigidbody()
	{
		mBodyId = b2_nullBodyId;
	}

	Rigidbody::~Rigidbody()
	{
		PhysicsManager::get().removeRigidbody(this);
		PhysicsManager::get().destroyBody(mBodyId);
	}

	void Rigidbody::onAttach()
	{
		b2BodyDef def = b2DefaultBodyDef();
		sf::Vector2f pos = mGameObject->mTransform.getPosition();
		def.position = { pos.x, pos.y };
		def.rotation = b2MakeRot(mGameObject->mTransform.getRotationRad());
		mBodyId = PhysicsManager::get().addRigidbody(this, &def);
		std::cout << "attached rb " << mBodyId.index1 << std::endl;
	}

	void Rigidbody::init()
	{

	}

	void Rigidbody::update(float dt)
	{

	}
	void Rigidbody::fixedUpdate()
	{

	}
}
