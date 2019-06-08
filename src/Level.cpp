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
		width = columns * TILE_WIDTH;
		height = rows * TILE_HEIGHT;
		for (double r = 0; r < rows; r++) {
			std::vector<DGameObject*> tmpVec;
			std::vector<UGameObject*> tmpVec2;
			for (double c = 0; c < columns; c++) {
				DGameObject* dobject = nullptr;
				UGameObject* uobject = nullptr;
				int code = levelData[int(r)][int(c)];
				if (code == GRASS) {
					dobject = new DGameObject("grass", { c * double(TILE_WIDTH), r * double(TILE_WIDTH) }, "resources/grass.png");
					dobject->setTilePosition({ c, r });
				} else if (code == DIRT) {
					dobject = new DGameObject("dirt", { c * double(TILE_WIDTH), r * double(TILE_WIDTH) }, "resources/dirt.png");
					dobject->setTilePosition({ c, r });
				} else if (code == 800) {
					uobject = new UGameObject("trigger1", { c * double(TILE_WIDTH), r * double(TILE_WIDTH) }, "create bridge");
					uobject->setTilePosition({ c, r });
				}
				tmpVec.push_back(dobject);
				tmpVec2.push_back(uobject);
			}
			drawableLevelObjects.push_back(tmpVec);
			updateableLevelObjects.push_back(tmpVec2);
		}
	}
}

void Level::drawLevel() {
	if (this != nullptr) {
		if (drawableLevelObjects.size() > 0) {
			for (int r = 0; r < drawableLevelObjects.size(); r++) {
				for (int c = 0; c < drawableLevelObjects[0].size(); c++) {
					if (drawableLevelObjects[r][c] != nullptr) {
						drawableLevelObjects[r][c]->draw();
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
