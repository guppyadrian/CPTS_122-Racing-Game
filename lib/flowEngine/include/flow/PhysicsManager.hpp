#pragma once

#include "components/Rigidbody.hpp"
#include <box2d/box2d.h>

namespace flow
{
	class PhysicsManager
	{
		b2WorldId mWorldId;
		float mAccum;
		const float fixedTimestep = 0.016f; // (60 tps)
		const int mQuality = 8; // substep count

		std::vector<Rigidbody*> mActiveRigidbodies;

		// Singleton Pattern
		PhysicsManager()
		{
			b2WorldDef wDef = b2DefaultWorldDef();
			wDef.gravity = { 0, 0 };
			mWorldId = b2CreateWorld(&wDef);
			mAccum = 0.f;
		};

		~PhysicsManager()
		{
			b2DestroyWorld(mWorldId);
		};

	public:

		PhysicsManager(const PhysicsManager&) = delete;
		PhysicsManager& operator=(const PhysicsManager&) = delete;
		static PhysicsManager& getGlobal() {
			static PhysicsManager Instance;
			return Instance;
		}

		void setGravity(sf::Vector2f gravity) {
			b2World_SetGravity(mWorldId, { gravity.x, gravity.y });
		};

		void tick(float dt);

		b2BodyId addRigidbody(Rigidbody* rb, b2BodyDef* bodyDef);
		void removeRigidbody(Rigidbody* rb);
		void destroyBody(b2BodyId bodyId);
	};
}