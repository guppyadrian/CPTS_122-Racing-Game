#include "flow/LevelScene.hpp"

namespace flow
{

	LevelScene::LevelScene(const std::string uuid)
		: Scene(uuid), _gameObjects()
	{
		// Scene base constructed with uuid
	}

	void LevelScene::initialize()
	{
		for (int i = 0; i < _gameObjects.size(); i++)
		{
			_gameObjects[i].init();
		}
	}

	void LevelScene::update(float dt)
	{
		for (int i = 0; i < _gameObjects.size(); i++)
		{
			_gameObjects[i].update(dt);
		}
	}

    void LevelScene::AddGameObject(flow::GameObject gameObject)
	{
		_gameObjects.push_back(std::move(gameObject));
	}
}