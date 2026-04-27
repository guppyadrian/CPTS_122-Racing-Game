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

		// AI Generated
		// Move constructor - ensure components' mGameObject pointers are updated
		GameObject(GameObject&& other) noexcept
			: mTransform(std::move(other.mTransform)), mComponents(std::move(other.mComponents))
		{
			for (auto& comp : mComponents)
			{
				comp->mGameObject = this;
			}
		}

		// AI Generated
		// Move assignment - ensure components' mGameObject pointers are updated
		GameObject& operator=(GameObject&& other) noexcept
		{
			if (this != &other)
			{
				mTransform = std::move(other.mTransform);
				mComponents = std::move(other.mComponents);
				for (auto& comp : mComponents)
				{
					comp->mGameObject = this;
				}
			}
			return *this;
		}

		flow::Transform mTransform;
		
		// get the first component of type T
		template<class T>
		T* getComponent();

		template <DerivedComponent T, typename... Args>
		T& addComponent(Args&&... args)
		{
			auto& component = *mComponents.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			component.mGameObject = this;
			return dynamic_cast<T&>(component);
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