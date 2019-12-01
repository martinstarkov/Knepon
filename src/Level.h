#pragma once
#include "TileMap.h"

extern const int TILE_WIDTH, TILE_HEIGHT;

class Level {
public:
	Level(std::string levelName, std::string mapFilePath);
	~Level();
	void printMapData();
	void drawLevel();

	GameObject* getTile(int x, int y) {
		if (x < levelObjects.size()) {
			if (y < levelObjects[x].size()) {
				return levelObjects[x][y];
			}
		}
		return nullptr;
	}
	GameObject* determineObject(int r, int c);
	void setCollideables();
	Vector2D entryPoint = { 0, 0 };
	Vector2D playerSpawn = { 0, 0 };
	int width = 0, height = 0;
	std::vector<std::vector<int>> levelData;
	std::vector<std::vector<GameObject*>> levelObjects;
	std::vector<GameObject*> collideableLevelObjects;
	std::vector<GameObject*> drawableLevelObjects;
	std::string name;
};