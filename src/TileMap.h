#pragma once

extern const int SCREEN_WIDTH, SCREEN_HEIGHT, TILE_WIDTH, TILE_HEIGHT;

class TileMap {
public:
	int zoomFactor = 3;
	int columns = SCREEN_WIDTH / TILE_WIDTH;
	int rows = SCREEN_HEIGHT / TILE_HEIGHT;
	std::vector<std::vector<int>> visibleTileMap;
};

