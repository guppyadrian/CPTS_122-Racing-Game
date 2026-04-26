#pragma once

#include <concepts>

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

	template <typename T>
	concept DerivedComponent = std::is_base_of_v<Component, T>;
}