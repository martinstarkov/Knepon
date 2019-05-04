#pragma once
#include "TileMap.h"

extern const int TILE_WIDTH, TILE_HEIGHT;

class Level {
public:
	Level(std::string levelName, std::string mapFilePath);
	~Level();
	void printMapData();
	void drawLevel();

	std::vector<std::vector<int>> mapData;
	std::string name;
};