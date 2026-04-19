#include "Scene.hpp"

Scene::Scene() : uuid("") {}

void Scene::onEnter() { return; }
void Scene::onExit() { return; }

LevelScene::LevelScene() : _gameObjects() {}