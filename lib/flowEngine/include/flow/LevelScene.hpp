#pragma once
#include "Scene.hpp"
#include "GameObject.hpp"

namespace flow
{
	class LevelScene : public Scene
	{
	private:
		std::vector<flow::GameObject> _gameObjects;
	public:
		LevelScene(const std::string uuid);
		~LevelScene() = default;
		void initialize();
		void update(float dt);
        // take by value and move into the internal vector to support non-copyable GameObject
		void AddGameObject(flow::GameObject gameObject);
		//TODO: Implement RemoveGameObject
	};
}