#include "common.h"

const int FPS = 60;
const float SKIP_TICK = 1000.0f / FPS;
extern const int SCREEN_WIDTH = 1200;
extern const int SCREEN_HEIGHT = 800;
SDL_Event event;

int main(int argc, char* args[]) {

	Uint32 frameStart, frameTime;
	Game* instance = Game::getInstance();

	if (instance->init("Knepon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) {
		std::cout << "Game succesfully started" << std::endl;
		
		while (instance->isRunning()) {

			frameStart = SDL_GetTicks();
			instance->handleEvents(&event);
			instance->update();
			instance->render();
			instance->tick++;

			frameTime = SDL_GetTicks() - frameStart;
			if (frameTime < SKIP_TICK) {
				SDL_Delay((int)(SKIP_TICK - frameTime));
			} else {
				SDL_Delay((int)SKIP_TICK);
			}

		}
	} else {
		std::cout << "Game failed to launch: " << SDL_GetError() << std::endl;
		return -1;
	}

	Game::getInstance()->quit();
	return 0;

}