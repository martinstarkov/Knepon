#include "common.h"

Game* Game::instance = 0;
Player* player;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL initialized successfully" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window != 0) {
			std::cout << "Window created successfully" << std::endl;
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer != 0) {
				std::cout << "Renderer created successfully" << std::endl;
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			} else {
				std::cout << "Renderer creation has failed" << std::endl;
				return false;
			}
		} else {
			std::cout << "Window creation has failed" << std::endl;
			return false;
		}
	} else {
		std::cout << "SDL initialization has failed" << std::endl;
		return false;
	}

	player = new Player("Player", "resources/slip.png", { SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 140, 280, 280 }, true, SDL_FLIP_NONE);

	std::cout << "Complete initialization successful" << std::endl;
	running = true;
	return true;
}

void Game::handleEvents(SDL_Event* event) {
	InputHandler::getInstance()->InputHandler::processInput(event);
}

void Game::update() {
	player->updatePosition();
	for (auto object : gameObjects) {
		object->updatePosition();
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	TextureManager::getInstance()->drawAnimated(player->id, (int)player->animationPhase, player->rectangle, renderer, player->direction);
	/*for (auto object : gameObjects) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_Rect rect = { object->hitbox.oldMin.x, object->hitbox.oldMax.y, object->hitbox.size.x, object->hitbox.size.y };
		SDL_RenderDrawRect(renderer, &rect);
	}*/
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game finished" << std::endl;
}

void Game::quit() {
	running = false;
}
