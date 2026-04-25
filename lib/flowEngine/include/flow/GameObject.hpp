#pragma once

#include <vector>
#include <memory>
#include <utility>
#include "Transform.hpp"
#include "Component.hpp"

namespace flow
{

	class GameObject
	{

	private:
		std::vector<std::unique_ptr<Component>> mComponents;
	public:
		GameObject() : mTransform() {};

		flow::Transform mTransform;
		
		// get the first component of type T
		template<class T>
		T* getComponent();

		template <DerivedComponent T, typename... Args>
		T& addComponent(Args&&... args)
		{
			mComponents.emplace_back<T>(std::forward<Args>(args)...);
			mComponents.back()->mGameObject = this;
			return dynamic_cast<T>(*mComponents.back());
		}

		void init(); // init all components
		void update(float dt); // update all components
		void fixedUpdate();

	};

	template<class T>
	inline T* GameObject::getComponent()
	{
		for (auto& component : mComponents)
		{
			if (T* derived = dynamic_cast<T*>(component.get()))
			{
				return derived;
			}
		}
		return nullptr;
	}
}