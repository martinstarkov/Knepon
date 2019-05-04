#include "common.h"

Game* Game::instance = 0;
extern const int SCREEN_WIDTH = 1008, SCREEN_HEIGHT = 608;
extern const int LEVEL_WIDTH = 1008, LEVEL_HEIGHT = 608;
extern const int TILE_WIDTH = 16, TILE_HEIGHT = 16;
Level* currentLevel;

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


	std::cout << "Complete initialization successful" << std::endl;
	running = true;
	return true;
}

void Game::handleEvents(SDL_Event* event) {
	InputHandler::getInstance()->InputHandler::processInput(event);
}

void Game::update() {

	switch (levelNumber) {
		case 1: {
			currentLevel = new Level("1", "F:\\Users\\Martin\\Desktop\\file.txt");
			break;
		}
		case 2: {
			currentLevel = new Level("2", "F:\\Users\\Martin\\Desktop\\level2.txt");
			break;
		}
		default: {
			break;
		}
	}

	Player::getInstance()->updatePosition();
	for (auto object : gameObjects) {
		object->updatePosition();
	}
}

void Game::render() {

	SDL_RenderClear(renderer);
	TextureManager::getInstance()->drawAnimated(Player::getInstance()->id, (int)Player::getInstance()->animationPhase, Player::getInstance()->rectangle, renderer, Player::getInstance()->direction);

	TextureManager::getInstance()->drawAnimated(Player::getInstance()->id, (int)Player::getInstance()->animationPhase, Player::getInstance()->rectangle, renderer, Player::getInstance()->direction);

	currentLevel->drawLevel();

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect playerHitbox = { Player::getInstance()->hitbox.min.x, Player::getInstance()->hitbox.max.y, Player::getInstance()->hitbox.size.w, Player::getInstance()->hitbox.size.h };
	SDL_RenderDrawRect(renderer, &playerHitbox);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (auto& rectangle : Player::getInstance()->collidingGrid) {
		SDL_RenderDrawRect(renderer, &rectangle);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	for (auto& object : gameObjects) {
		SDL_Rect rect = { object->hitbox.oldMin.x, object->hitbox.oldMax.y, object->hitbox.size.w, object->hitbox.size.h };
		SDL_RenderDrawRect(renderer, &rect);
	}

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
