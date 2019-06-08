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
		if (x < drawableLevelObjects.size()) {
			if (y < drawableLevelObjects[x].size()) {
				return drawableLevelObjects[x][y];
			}
		}
		return nullptr;
	}
	int width, height;
	std::vector<std::vector<int>> levelData;
	std::vector<std::vector<DGameObject*>> drawableLevelObjects;
	std::vector<std::vector<UGameObject*>> updateableLevelObjects;
	std::string name;
};