#include "common.h"

const int FPS = 60;
const float SKIP_TICK = 1000.0f / FPS;
extern const double SCREEN_WIDTH = 1200;
extern const double SCREEN_HEIGHT = 800;
extern const double LEVEL_WIDTH = 1700;
extern const double LEVEL_HEIGHT = 900;

SDL_Event event;

int main(int argc, char* args[]) {

	Uint32 frameStart, frameTime;

	if (Game::getInstance()->init("Knepon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) {
		std::cout << "Game succesfully started" << std::endl;
		
		while (Game::getInstance()->isRunning()) {
			frameStart = SDL_GetTicks();

			Game::getInstance()->handleEvents(event);
			Game::getInstance()->update();
			Game::getInstance()->render();
			Game::getInstance()->tick++;

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