/**
 * @file LevelScene.hpp
 * @author Johnathan Emanuelli
 * @author Logan Rainchild
 * @brief a Scene that stores a list of gameObjects.
 */

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
		~LevelScene() override = default;
		void onEnter() override;
		void onExit() override;
		// take by value and move into the internal vector to support non-copyable GameObject
		void initialize() override;
		void update(float dt) override;
		void draw() override;
        // take by value and move into the internal vector to support non-copyable GameObject
		GameObject& AddGameObject(flow::GameObject gameObject);
		//TODO: Implement RemoveGameObject
	};
}