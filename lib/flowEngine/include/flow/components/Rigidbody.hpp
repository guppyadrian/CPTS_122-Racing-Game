/**
 * @file RigidBOdy.hpp
 * @author Logan Rainchild
 * @brief A component that provides box2d rigidbody functions for gameObjects
 */

#pragma once

#include "flow/Component.hpp"
#include "flow/GameObject.hpp"
#include <box2d/box2d.h>

namespace flow
{
	class Rigidbody : public Component
	{
		sf::Vector2f mLastPos;
		float mLastRot;
		b2BodyId mBodyId;

	public:

		Rigidbody();
		~Rigidbody();

		// used for adding shapes to the rigidbody for collisions
		b2BodyId getBodyId()
		{
			return mBodyId;
		};

		sf::Vector2f getLastPosition()
		{
			return mLastPos;
		}

		float getLastRotation()
		{
			 return mLastRot;
		}

		void setLastPosition(sf::Vector2f pos)
		{
			mLastPos = pos;
		};
		void setLastRotation(float rot)
		{
			mLastRot = rot;
		};

		void resetInterpolation()
		{
			auto pos = b2Body_GetPosition(mBodyId);
			mLastPos = { pos.x, pos.y };
			auto rot = b2Body_GetRotation(mBodyId);
			mLastRot = b2Rot_GetAngle(rot);
		}

		void init() override;
		void update(float dt) override;
		void fixedUpdate() override;
	};
}