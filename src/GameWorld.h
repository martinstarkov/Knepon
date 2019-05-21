#pragma once
#include "GameObject.h"

class GameWorld {
public:
	GameWorld();
	~GameWorld();
	static GameWorld* getInstance() {
		if (instance == 0) {
			instance = new GameWorld();
		}
		return instance;
	}
	static Level* getCurrentLevel() {
		return currentLevel;
	}
	static void setCurrentLevel(std::string levelName);
	static std::vector<DGameObject*> drawableObjects;
	static std::vector<GameObject*> customObject;
	static std::vector<UGameObject*> updateableObjects;
	static std::map<std::string, Level*> levels;
private:
	static GameWorld* instance;
	static Level* currentLevel;
};

