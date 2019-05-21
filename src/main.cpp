#include "common.h"

const int SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 800;
const double FPS = 120;
Game* instance = Game::getInstance();

int main(int argc, char* args[]) {

	if (instance->init("Knepon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) {

		std::cout << "Game succesfully started" << std::endl;

		SDL_Event event;

		Uint32 previousFrame;
		Uint32 currentFrame = SDL_GetTicks();

		while (instance->isRunning()) {

			previousFrame = currentFrame;
			currentFrame = SDL_GetTicks();
			double dt = (currentFrame - previousFrame) / 1000.0f;

			instance->handleEvents(&event);

			instance->update(dt);

			instance->render();

		}

	} else {
		std::cout << "Game failed to launch: " << SDL_GetError() << std::endl;
		return -1;
	}

	instance->quit();
	return 0;

}