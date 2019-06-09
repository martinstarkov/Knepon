#include "common.h"

GameWorld* GameWorld::instance;
std::vector<DGameObject*> GameWorld::drawableObjects;
std::vector<UGameObject*> GameWorld::updateableObjects;
std::vector<GameObject*> GameWorld::customObject;
std::map<std::string, Level*> GameWorld::levels;
Level* GameWorld::currentLevel = nullptr;

GameWorld::GameWorld() {}
GameWorld::~GameWorld() {}

void GameWorld::setCurrentLevel(std::string levelName) {
	if (levels.find(levelName) != levels.end()) {
		std::cout << levelName << " set as current level" << std::endl;
		currentLevel = levels[levelName];
	} else {
		std::cout << "Level does not exist" << std::endl;
		currentLevel = nullptr;
	}
}