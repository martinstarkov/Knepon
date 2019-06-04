#include "common.h"

Game* Game::instance;
SDL_Window* Game::window;
SDL_Renderer* Game::renderer;
bool Game::running;

extern const int SCREEN_WIDTH, SCREEN_HEIGHT;
const int TILE_WIDTH = 16, TILE_HEIGHT = 16;
Player* player;
DGameObject* myobj1;
DGameObject* myobj2;
DGameObject* myobj3;

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

	myobj1 = new DGameObject("top dirt", { 264, 248 }, "resources/dirt.png");
	myobj2 = new DGameObject("corner dirt", { 264, 264 }, "resources/dirt.png");
	myobj3 = new DGameObject("side dirt", { 248, 264 }, "resources/dirt.png");
	GameWorld::customObject.push_back(myobj3);
	GameWorld::customObject.push_back(myobj2);
	GameWorld::customObject.push_back(myobj1);

	player = Player::getInstance();

	Level* level1 = new Level("level1", "levels/level1.txt");
	GameWorld::setCurrentLevel(level1->name);
	return true;
}

void Game::handleEvents(SDL_Event* event) {
	InputHandler::processInput(event);
}

void Game::update(double dt) {
	player->update(dt);
}

void Game::render() {
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect rect = { (int)player->getPosition().x, (int)player->getPosition().y, (int)player->getSize().x, (int)player->getSize().y };
	SDL_RenderDrawRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect rect2 = { (int)myobj1->getPosition().x, (int)myobj1->getPosition().y, (int)myobj1->getSize().x, (int)myobj1->getSize().y };
	SDL_RenderDrawRect(renderer, &rect2);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect rect3 = { (int)myobj2->getPosition().x, (int)myobj2->getPosition().y, (int)myobj2->getSize().x, (int)myobj2->getSize().y };
	SDL_RenderDrawRect(renderer, &rect3);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect rect4 = { (int)myobj3->getPosition().x, (int)myobj3->getPosition().y, (int)myobj3->getSize().x, (int)myobj3->getSize().y };
	SDL_RenderDrawRect(renderer, &rect4);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &DUGameObject::mBox);

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
