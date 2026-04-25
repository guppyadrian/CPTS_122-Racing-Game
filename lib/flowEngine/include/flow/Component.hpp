#pragma once

namespace flow
{
	class GameObject;

	class Component
	{
	public:
		GameObject* mGameObject = nullptr;
		virtual ~Component() = default;
		virtual void init() = 0;
		virtual void update(float dt) = 0;
		virtual void fixedUpdate() = 0;
	};
}