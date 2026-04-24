#pragma once

#include <vector>
#include <memory>
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
		void addComponent(std::unique_ptr<Component> c)
		{
           // set the parent pointer on the component and store it
			c->mGameObject = this;
			mComponents.push_back(std::move(c));
		};

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