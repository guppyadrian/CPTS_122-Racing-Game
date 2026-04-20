#pragma once

#include "Rigidbody.hpp"
#include <box2d/box2d.h>

namespace flow
{
	class PhysicsManager
	{
		b2WorldId mWorldId;
		float mAccum;
		const float fixedTimestep = 20; // 20 ms between fixedUpdate (50 tps)

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
		static PhysicsManager& get() {
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