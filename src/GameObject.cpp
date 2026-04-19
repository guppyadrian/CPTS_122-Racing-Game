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

	void GameObject::update()
	{
		for (auto& component : mComponents)
		{
			component->update();
		}
	}
}
