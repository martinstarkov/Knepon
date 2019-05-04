#include "common.h"

Level::Level(std::string levelName, std::string mapFilePath) {
	name = levelName;
	Game::getInstance()->levels[levelName] = this;
	std::ifstream file(mapFilePath);
	if (!file) {
		std::cout << "Error, file " << mapFilePath << " could not be loaded into level " << levelName << "." << std::endl;
	} else {
		int rows = 0, columns = 0;
		std::string line, item;
		while (getline(file, line)) {
			rows++;
			std::vector<int> row;
			std::stringstream lineString(line);
			int value;
			while (lineString >> item) {
				if (rows == 1) columns++;
				std::istringstream iss(item);
				iss >> value;
				row.push_back(value);
			}
			mapData.push_back(row);
		}
		Game::getInstance()->gameObjects.clear();
		if (mapData.size() > 0) {
			for (int r = 0; r < mapData.size(); r++) {
				for (int c = 0; c < mapData[0].size(); c++) {
					if (mapData[r][c] == GRASS) {
						GameObject* grass = new GameObject("grass", "resources/grass.png", { (c * TILE_WIDTH), (r * TILE_HEIGHT), TILE_WIDTH, TILE_HEIGHT }, false, SDL_FLIP_NONE);
					} else if (mapData[r][c] == DIRT) {
						GameObject* dirt = new GameObject("dirt", "resources/dirt.png", { (c * TILE_WIDTH), (r * TILE_HEIGHT), TILE_WIDTH, TILE_HEIGHT }, false, SDL_FLIP_NONE);
					}
				}
			}
		}
	}
}

void Level::drawLevel() {
	if (mapData.size() > 0) {
		for (int r = 0; r < mapData.size(); r++) {
			for (int c = 0; c < mapData[0].size(); c++) {
				if (mapData[r][c] == GRASS) {
					TextureManager::getInstance()->draw("grass", { (c * TILE_WIDTH), (r * TILE_HEIGHT), TILE_WIDTH, TILE_HEIGHT }, Game::getInstance()->getRenderer(), SDL_FLIP_NONE);
				} else if (mapData[r][c] == DIRT) {
					TextureManager::getInstance()->draw("dirt", { (c * TILE_WIDTH), (r * TILE_HEIGHT), TILE_WIDTH, TILE_HEIGHT }, Game::getInstance()->getRenderer(), SDL_FLIP_NONE);
				}
			}
		}
	} else {
		std::cout << "Unable to draw level " << name << " as map data is incomplete" << std::endl;
	}
}

void Level::printMapData() {
	std::cout << name << " layout:" << std::endl;
	for (int r = 0; r < mapData.size(); r++) {
		for (int c = 0; c < mapData[0].size(); c++) {
			std::cout << mapData[r][c];
		}
		std::cout << std::endl;
	}
}

Level::~Level() {
}
