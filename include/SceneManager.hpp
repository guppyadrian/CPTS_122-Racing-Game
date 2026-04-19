#pragma once
#include <vector>
#include "Scene.hpp"


class SceneManager
{
private:
	Scene* _curScene;
	std::vector<Scene> _sceneList;

public:
	SceneManager();
	~SceneManager();
	void update();
	bool switchScene(const int index);
	void addScene(Scene& newScene);
	bool removeScene(const int index);
};
