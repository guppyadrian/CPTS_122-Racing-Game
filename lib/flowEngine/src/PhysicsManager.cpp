#include <algorithm>

#include "flow/PhysicsManager.hpp"
#include "flow/Interpolation.hpp"
#include <SFML/System/Vector2.hpp>

namespace flow
{
	void PhysicsManager::tick(float dt)
	{
		mAccum += std::min(dt, 0.5f); // clamp to (0.5 seconds max)

		// if the accumulation threshold is not met physics does not update
		// if the framerate is poor and physics needs to update at a higher frequency call multiple steps per frame
		while (mAccum >= fixedTimestep)
		{
			// set the last position of active rigidbodies
			for (auto* rb : mActiveRigidbodies)
			{
				auto pos = b2Body_GetPosition(rb->getBodyId());
				rb->setLastPosition(sf::Vector2f(pos.x, pos.y));
				auto rot = b2Body_GetRotation(rb->getBodyId());
				rb->setLastRotation(b2Rot_GetAngle(rot));
			}

			for (auto* rb : mActiveRigidbodies)
			{
				
				// Implement the following line if transform athority is desirable
				//b2Body_SetTransform(rb->getBodyId(), rb->mGameObject->mTransform.getPosition(), rb->mGameObject->mTransform.getRotation())

				rb->mGameObject->fixedUpdate();
			}
			b2World_Step(mWorldId, fixedTimestep, mQuality);

			mAccum -= fixedTimestep;
		}

		// interpolate rigidbody position

		float t = (mAccum / fixedTimestep);
		t = std::clamp(t, 0.0f, 1.0f);

		for (auto* rb : mActiveRigidbodies)
		{
			b2Vec2 pos = b2Body_GetPosition(rb->getBodyId());
			float rot = b2Rot_GetAngle(b2Body_GetRotation(rb->getBodyId()));

			sf::Vector2f iPosition = interp::lerp(rb->getLastPosition(), sf::Vector2f(pos.x, pos.y), t);
			float iRotation = interp::slerp(rb->getLastRotation(), rot, t);

			rb->mGameObject->mTransform.setPosition(iPosition);
			rb->mGameObject->mTransform.setRotationRad(iRotation);
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