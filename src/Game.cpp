#include "common.h"

Game* Game::instance;
SDL_Window* Game::window;
SDL_Renderer* Game::renderer;
bool Game::running;

extern const int SCREEN_WIDTH, SCREEN_HEIGHT;
const int TILE_WIDTH = 16, TILE_HEIGHT = 16;
const Vector2D TILE_SIZE = { double(TILE_WIDTH), double(TILE_WIDTH) };
Player* player;
SDL_Rect Game::camera;

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

	player = Player::getInstance();

	Level* level1 = new Level("level1", "levels/level1.txt");
	Level* level2 = new Level("level2", "levels/level2.txt");
	GameWorld::setCurrentLevel(level1->name);
	player->setPosition(GameWorld::getCurrentLevel()->playerSpawn);
	camera = { 0, 0, GameWorld::getCurrentLevel()->width, GameWorld::getCurrentLevel()->height };
	
	return true;
}

void Game::handleEvents(SDL_Event* event) {
	InputHandler::processInput(event);
}

void Game::update(double dt) {
	player->update(dt);
	camera.x = (int)player->getPosition().x - SCREEN_WIDTH / 2;
	camera.y = (int)player->getPosition().y - SCREEN_HEIGHT / 2;
	if (camera.w >= SCREEN_WIDTH) {
		if (camera.x < 0) {
			camera.x = 0;
		}
		if (camera.x > GameWorld::getCurrentLevel()->width - SCREEN_WIDTH) {
			camera.x = GameWorld::getCurrentLevel()->width - SCREEN_WIDTH;
		}
	}
	if (camera.h >= SCREEN_HEIGHT) {
		if (camera.y < 0) {
			camera.y = 0;
		}
		if (camera.y > GameWorld::getCurrentLevel()->height - SCREEN_HEIGHT) {
			camera.y = GameWorld::getCurrentLevel()->height - SCREEN_HEIGHT;
		}
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	
	for (auto& object : GameWorld::getCurrentLevel()->collideableLevelObjects) {
		object->runClassSpecific();
	}
	//GameWorld::getCurrentLevel()->drawLevel();
	player->draw();
	SDL_Rect tileSquare = { int(player->getTilePosition().x) * TILE_WIDTH - camera.x, int(player->getTilePosition().y) * TILE_HEIGHT - camera.y, TILE_WIDTH, TILE_HEIGHT };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderDrawRect(renderer, &tileSquare);

	SDL_SetRenderDrawColor(renderer, 195, 198, 186, 255);
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
