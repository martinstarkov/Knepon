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
	static SDL_Renderer* getRenderer() {
		return renderer;
	}
	static SDL_Window* getWindow() {
		return window;
	}
	static bool isRunning() {
		return running;
	}
	bool init(const char* title, int x, int y, int width, int height, int flags);
	void handleEvents(SDL_Event* event);
	void update(double dt);
	void render();
	void clean();
	void quit();
	static SDL_Rect camera;
private:
	static Game* instance;
	static bool running;
	static SDL_Window* window;
	static SDL_Renderer* renderer;
};