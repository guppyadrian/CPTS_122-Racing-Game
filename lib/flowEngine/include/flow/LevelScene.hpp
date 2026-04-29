#pragma once
#include "Scene.hpp"
#include "GameObject.hpp"

namespace flow
{
	class LevelScene : public Scene
	{
	private:
		std::vector<flow::GameObject> _gameObjects;
		bool hasInitialized = false;
	public:
		LevelScene(const std::string uuid);
		~LevelScene() = default;
		void onEnter() override;
		void onExit() override;
		void initialize();
		void update(float dt);
		// take by value and move into the internal vector to support non-copyable GameObject
		GameObject& AddGameObject(flow::GameObject gameObject);
		//TODO: Implement RemoveGameObject
	};
}