#include "Rigidbody.hpp"
#include "PhysicsManager.hpp"

namespace flow
{
	Rigidbody::Rigidbody()
	{
		b2BodyDef def = b2DefaultBodyDef();
		sf::Vector2f pos = mGameObject->mTransform.getPosition();
		def.position = {pos.x, pos.y};
		def.rotation = { mGameObject->mTransform.getRotationRad() };
		mBodyId =  PhysicsManager::get().addRigidbody(this, &def);
	}

	Rigidbody::~Rigidbody()
	{
		PhysicsManager::get().removeRigidbody(this);
		PhysicsManager::get().destroyBody(mBodyId);
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
