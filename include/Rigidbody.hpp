#pragma once

#include "GameObject.hpp"
#include <box2d/box2d.h>

namespace flow
{
	class Rigidbody : public Component
	{
		b2BodyId mBodyId;

	public:

		Rigidbody();
		~Rigidbody();

		// used for adding shapes to the rigidbody for collisions
		b2BodyId getBodyId()
		{
			return mBodyId;
		};

		void init() override;
		void update(float dt) override;
		void fixedUpdate() override;
	};
}