#pragma once
#include "GameObject.h"
#include "Level.h"

class Game {
public:
	static Game* getInstance() {
		if (instance == 0) {
			instance = new Game();
		}
		return instance;
	}
	SDL_Renderer* getRenderer() {
		return renderer;
	}
	SDL_Window* getWindow() {
		return window;
	}
	bool isRunning() { 
		return running;
	}
	bool init(const char* title, int x, int y, int width, int height, int flags);
	void handleEvents(SDL_Event* event);
	void update();
	void render();
	void clean();
	void quit();

	int levelNumber = 1;
	int tick = 0;
	std::vector<GameObject*> gameObjects;
	std::map<std::string, Level*> levels;
private:
	static Game* instance;
	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

