#include "flow/LevelScene.hpp"

#include "flow/Renderer.hpp"

namespace flow
{

	LevelScene::LevelScene(const std::string uuid)
		: Scene(uuid), _gameObjects()
	{
		// Scene base constructed with uuid
	}

	void LevelScene::onEnter()
	{
		initialize();
	}

	void LevelScene::onExit()
	{
	}

	void LevelScene::initialize()
	{
		for (int i = 0; i < _gameObjects.size(); i++)
		{
			_gameObjects[i].init();
		}
		hasInitialized = true;
	}

	void LevelScene::update(float dt)
	{
		auto& window = Renderer::getGlobalRenderer().getWindow();
		while (const auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}
		for (int i = 0; i < _gameObjects.size(); i++)
		{
			_gameObjects[i].update(dt);
		}
	}

	void LevelScene::draw()
	{
		auto& window = Renderer::getGlobalRenderer().getWindow();
		window.clear();
		
		Renderer::getGlobalRenderer().drawAll();
		
		window.display();
	}

	GameObject& LevelScene::AddGameObject(flow::GameObject gameObject)
	{
		_gameObjects.push_back(std::move(gameObject));
		if (hasInitialized)
		{
			_gameObjects.back().init();
		}
		return _gameObjects.back();
	}
}
