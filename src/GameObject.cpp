#include "GameObject.hpp"

namespace flow
{
	void GameObject::init()
	{
		for (auto &component : mComponents)
		{
			component->init();
		}
	}

	void GameObject::update(float dt)
	{
		for (auto& component : mComponents)
		{
			component->update(dt);
		}
	}

	void GameObject::fixedUpdate()
	{
		for (auto& component : mComponents)
		{
			component->fixedUpdate();
		}
	}
}
