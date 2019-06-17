#include "common.h"

const int SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 800;
const double FPS = 120;
const double UPS = 60;
const double UPDATE_TICK = 1000 / FPS;
Game* instance = Game::getInstance();

int main(int argc, char* args[]) {

	if (instance->init("Knepon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) {

		std::cout << "Game succesfully started" << std::endl;

		SDL_Event event;

		//Uint32 previousFrame;
		//Uint32 currentFrame = SDL_GetTicks();
		Uint32 lastTime = SDL_GetTicks();

		while (instance->isRunning()) {

			//previousFrame = currentFrame;
			//currentFrame = SDL_GetTicks();
			//double dt = (currentFrame - previousFrame) / 1000.0f;

			Uint32 time = SDL_GetTicks();
			if (time - lastTime > 1000) {
				lastTime = time;  // we're too far behind, catch up
			}
			int updatesNeeded = (time - lastTime) / UPDATE_TICK;

			instance->handleEvents(&event);

			for (int i = 0; i < updatesNeeded; i++) {
				instance->update(0);
				lastTime += UPDATE_TICK;
			}

			instance->render();
			
			/*
			if (currentFrame - SDL_GetTicks() < SKIP_TICK) {
				SDL_Delay(SKIP_TICK - (currentFrame - SDL_GetTicks()));
			}
			*/
		}

	} else {
		std::cout << "Game failed to launch: " << SDL_GetError() << std::endl;
		return -1;
	}

	instance->quit();
	return 0;

}