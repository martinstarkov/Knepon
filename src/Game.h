#pragma once
#include "GameObject.h"

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
	std::vector<GameObject*> gameObjects;
	bool init(const char* title, int x, int y, int width, int height, int flags);
	void handleEvents(SDL_Event* event);
	void update();
	void render();
	void clean();
	void quit();
	int tick = 0;
private:
	static Game* instance;
	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

