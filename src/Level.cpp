#include "common.h"

Level::Level(std::string levelName, std::string mapFilePath) : name(levelName) {
	GameWorld::levels[levelName] = this;
	std::ifstream file(mapFilePath);
	if (!file) {
		std::cout << "Error, file " << mapFilePath << " could not be loaded into level " << levelName << "." << std::endl;
	} else {
		double rows = 0, columns = 0;
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
			levelData.push_back(row);
		}
		width = (int)columns * TILE_WIDTH;
		height = (int)rows * TILE_HEIGHT;
		for (double r = 0; r < rows; r++) {
			std::vector<GameObject*> tmpVec;
			for (double c = 0; c < columns; c++) {
				GameObject* object = nullptr;
				int code = levelData[int(r)][int(c)];
				if (code == PLAYERSPAWN) {
					playerSpawn = { c * double(TILE_WIDTH), r * double(TILE_WIDTH) };
				} else if (code == GRASS) {
					object = new DGameObject("grass", { c * double(TILE_WIDTH), r * double(TILE_WIDTH) }, { double(TILE_WIDTH), double(TILE_WIDTH) }, "resources/grass.png");
				} else if (code == DIRT) {
					object = new DGameObject("dirt", { c * double(TILE_WIDTH), r * double(TILE_WIDTH) }, { double(TILE_WIDTH), double(TILE_WIDTH) }, "resources/dirt.png");
				} else if (code == PUMPKIN) {
					object = new DGameObject("pumpkin", { c * double(TILE_WIDTH), r * double(TILE_WIDTH) }, { double(TILE_WIDTH), double(TILE_WIDTH) }, "resources/pumpkin.png");
				} else if (code == STONE) {
					object = new DGameObject("stone", { c * double(TILE_WIDTH), r * double(TILE_WIDTH) }, { double(TILE_WIDTH), double(TILE_WIDTH) }, "resources/stone.png");
				} else if (code == SNOW) {
					object = new DGameObject("dirt", { c * double(TILE_WIDTH), r * double(TILE_WIDTH) }, { double(TILE_WIDTH), double(TILE_WIDTH) }, "resources/snow.png");
				} else if (code == DOOR) {
					entryPoint = { c * double(TILE_WIDTH), r * double(TILE_WIDTH) };
					object = new DGameObject("door", { c * double(TILE_WIDTH), r * double(TILE_WIDTH) }, { double(TILE_WIDTH), double(TILE_WIDTH) }, "resources/door.png", true);
				}
				object->setTilePosition({ c, r });
				tmpVec.push_back(object);
			}
			levelObjects.push_back(tmpVec);
		}
		std::cout << "Successfully loaded level: """ << name << """, [" << columns << "x" << rows << "] tiles" << std::endl;
	}
}

void Level::drawLevel() {
	if (this != nullptr) {
		if (levelObjects.size() > 0) {
			for (int r = 0; r < levelObjects.size(); r++) {
				for (int c = 0; c < levelObjects[0].size(); c++) {
					if (levelObjects[r][c] != nullptr) {
						levelObjects[r][c]->runClassSpecific();
					}
				}
			}
		} else {
			std::cout << "Unable to draw level " << name << " as map data is incomplete" << std::endl;
		}
	} else {
		std::cout << "Please set a current level before attempting to drawing one." << std::endl;
	}
}

void Level::printMapData() {
	std::cout << name << " layout:" << std::endl;
	for (int r = 0; r < levelData.size(); r++) {
		for (int c = 0; c < levelData[0].size(); c++) {
			std::cout << levelData[r][c];
		}
		std::cout << std::endl;
	}
}

Level::~Level() {
}
