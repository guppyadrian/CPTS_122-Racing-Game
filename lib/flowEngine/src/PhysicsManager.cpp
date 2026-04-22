#include "flow/PhysicsManager.hpp"

namespace flow
{
	void PhysicsManager::tick(float dt)
	{
		mAccum += std::min(dt, 0.5f); // clamp to (0.5 seconds max)

		// if the accumulation threshold is not met physics does not update
		// if the framerate is poor and physics needs to update at a higher frequency call multiple steps per frame
		while (mAccum >= fixedTimestep)
		{
			for (auto* rb : mActiveRigidbodies)
			{
				
				// Implement the following line if transform athority is desirable
				//b2Body_SetTransform(rb->getBodyId(), rb->mGameObject->mTransform.getPosition(), rb->mGameObject->mTransform.getRotation())

				rb->mGameObject->fixedUpdate();
			}
			b2World_Step(mWorldId, fixedTimestep, mQuality);

			// the rigidbody simulation is authoritative over the gameobject's transforms
			for (auto* rb : mActiveRigidbodies)
			{
				auto pos = b2Body_GetPosition(rb->getBodyId());
				rb->mGameObject->mTransform.setPosition(sf::Vector2f(pos.x, pos.y));
				auto rot = b2Body_GetRotation(rb->getBodyId());
				rb->mGameObject->mTransform.setRotationRad(b2Rot_GetAngle(rot));
			}

			mAccum -= fixedTimestep;
		}
	}

	b2BodyId PhysicsManager::addRigidbody(Rigidbody* rb, b2BodyDef *bodyDef)
	{
		b2BodyId bId = b2CreateBody(mWorldId, bodyDef);
		mActiveRigidbodies.push_back(rb);
		return bId;
	}
	void PhysicsManager::removeRigidbody(Rigidbody* rb)
	{
		auto it = std::find(mActiveRigidbodies.begin(), mActiveRigidbodies.end(), rb);
		if (it != mActiveRigidbodies.end())
		{
			mActiveRigidbodies.erase(it);
		}
	}
	void PhysicsManager::destroyBody(b2BodyId bodyId)
	{
		b2DestroyBody(bodyId);
	}
}