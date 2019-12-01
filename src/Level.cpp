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
				GameObject* object = determineObject(int(r), int(c));
				object->setTilePosition({ c, r });
				tmpVec.push_back(object);
			}
			levelObjects.push_back(tmpVec);
		}
		setCollideables();
		std::cout << "Successfully loaded level: """ << name << """, [" << columns << "x" << rows << "] tiles, " << collideableLevelObjects.size() << " collideable objects" << std::endl;
	}
}

GameObject* Level::determineObject(int r, int c) {
	Vector2D coordinate = { double(c) * double(TILE_WIDTH), double(r) * double(TILE_WIDTH) };
	switch (levelData[r][c]) {
		case PLAYERSPAWN:
			playerSpawn = coordinate;
			break;
		case GRASS:
			return new DGameObject("grass", coordinate, "resources/grass.png");
		case DIRT:
			return new DGameObject("dirt", coordinate, "resources/dirt.png");
		case PUMPKIN:
			return new DGameObject("pumpkin", coordinate, "resources/pumpkin.png");
		case STONE:
			return new DGameObject("stone", coordinate, "resources/stone.png");
		case SNOW:
			return new DGameObject("snow", coordinate, "resources/snow.png");
		case DOOR:
			entryPoint = coordinate;
			return new DGameObject("door", coordinate, "resources/door.png", TILE_SIZE, true);
	}
	return nullptr;
}

void Level::setCollideables() {
	//collideableLevelObjects
	if (levelObjects.size() > 0) {
		for (int r = 0; r < levelObjects.size(); r++) {
			for (int c = 0; c < levelObjects[0].size(); c++) {
				if (levelObjects[r][c] != nullptr) {
					if (getTile(r + 1, c) != nullptr && getTile(r + 1, c)->isInteractable() == false && getTile(r - 1, c) != nullptr && getTile(r - 1, c)->isInteractable() == false && getTile(r, c + 1) != nullptr && getTile(r, c + 1)->isInteractable() == false && getTile(r, c - 1) != nullptr && getTile(r, c - 1)->isInteractable() == false) {
						//landlocked object
					} else {
						collideableLevelObjects.push_back(levelObjects[r][c]);
					}
					drawableLevelObjects.push_back(levelObjects[r][c]);
				}
			}
		}
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
