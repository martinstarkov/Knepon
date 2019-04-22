#include "common.h"

Game* Game::instance = 0;
GameObject* object1;
GameObject* object2;
GameObject* object3;
GameObject* object4;
GameObject* object5;
extern const double SCREEN_WIDTH;
extern const double SCREEN_HEIGHT;
extern const double LEVEL_HEIGHT;
extern const double LEVEL_WIDTH;

SDL_Rect Player::camera = { 0, 0, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT };

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

	object1 = new GameObject("grass", "resources/grass.png", { SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60, 280, 280 }, false);
	object2 = new GameObject("grass2", "resources/grass.png", { SCREEN_WIDTH / 2 + 280, SCREEN_HEIGHT - 30, 280, 280 }, false);
	object3 = new GameObject("grass3", "resources/grass.png", { SCREEN_WIDTH / 2 + 560, SCREEN_HEIGHT - 30, 280, 280 }, false);
	object4 = new GameObject("grass4", "resources/grass.png", { SCREEN_WIDTH / 2 + 840, SCREEN_HEIGHT - 240, 280, 280 }, false);
	object5 = new GameObject("grass5", "resources/grass.png", { SCREEN_WIDTH / 2 + 840, SCREEN_HEIGHT - 300, 280, 280 }, false);

	std::cout << "Complete initialization successful" << std::endl;
	running = true;
	return true;
}

/*
void Game::gravity() {
	for (auto i : gameObjects) {
		if (i->falling || i->jumping) {
			i->yspeed += gravityConstant;
		}
	}
}
*/

void Game::handleEvents(SDL_Event event) {
	InputHandler::getInstance()->InputHandler::processInput(event);
}

void Game::update() {
	Player::getInstance()->updatePosition();
	for (auto i : gameObjects) {
		i->updatePosition();
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	//draw player texture
	if (Player::getInstance() != 0) {
		TextureManager::getInstance()->animate(Player::getInstance()->id, Player::getInstance()->animationPhase, Player::getInstance()->player, renderer, Player::getInstance()->direction);
	}
	TextureManager::getInstance()->draw(object1->id, object1->rectangle, renderer, SDL_FLIP_NONE);
	TextureManager::getInstance()->draw(object2->id, object2->rectangle, renderer, SDL_FLIP_NONE);
	TextureManager::getInstance()->draw(object3->id, object3->rectangle, renderer, SDL_FLIP_NONE);
	TextureManager::getInstance()->draw(object4->id, object4->rectangle, renderer, SDL_FLIP_NONE);
	TextureManager::getInstance()->draw(object5->id, object5->rectangle, renderer, SDL_FLIP_NONE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &Player::getInstance()->camera);
	for (auto i : gameObjects) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		//SDL_Rect rect = { i->hitbox.min.x, i->hitbox.max.y, i->hitbox.size.x, i->hitbox.size.y };
		SDL_Rect rect = { i->hitbox.oldMin.x, i->hitbox.oldMax.y, i->hitbox.size.x, i->hitbox.size.y };
		//SDL_Rect rect = { i->rectangle.x, i->rectangle.y, i->rectangle.w, i->rectangle.h };
		SDL_RenderDrawRect(renderer, &rect);
	}
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_Rect rect = { Player::getInstance()->hitbox.oldMin.x, Player::getInstance()->hitbox.oldMax.y, Player::getInstance()->player.w, Player::getInstance()->player.h };
	SDL_RenderDrawRect(renderer, &rect);

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
