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
DGameObject* myobj4;
DGameObject* myobj5;
DGameObject* myobj6;
DGameObject* myobj7;
DGameObject* myobj8;
DGameObject* myobj9;
DGameObject* myobj10;
DGameObject* myobj11;
DGameObject* myobj12;
DGameObject* myobj13;

/*
bool ShapeOverlap_SAT_STATIC(Polygon& r1, Polygon& r2) {
	Polygon* poly1 = &r1;
	Polygon* poly2 = &r2;

	float overlap = INFINITY;

	for (int shape = 0; shape < 2; shape++) {
		if (shape == 1) {
			poly1 = &r2;
			poly2 = &r1;
		}

		for (int a = 0; a < poly1->p.size(); a++) {
			int b = (a + 1) % poly1->p.size();
			Vector2D axisProj = { -(poly1->p[b].y - poly1->p[a].y), poly1->p[b].x - poly1->p[a].x };

			// Optional normalisation of projection axis enhances stability slightly
			//float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
			//axisProj = { axisProj.x / d, axisProj.y / d };

			// Work out min and max 1D points for r1
			float min_r1 = INFINITY, max_r1 = -INFINITY;
			for (int p = 0; p < poly1->p.size(); p++) {
				float q = (poly1->p[p].x * axisProj.x + poly1->p[p].y * axisProj.y);
				min_r1 = std::min(min_r1, q);
				max_r1 = std::max(max_r1, q);
			}

			// Work out min and max 1D points for r2
			float min_r2 = INFINITY, max_r2 = -INFINITY;
			for (int p = 0; p < poly2->p.size(); p++) {
				float q = (poly2->p[p].x * axisProj.x + poly2->p[p].y * axisProj.y);
				min_r2 = std::min(min_r2, q);
				max_r2 = std::max(max_r2, q);
			}

			// Calculate actual overlap along projected axis, and store the minimum
			overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

			if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
				return false;
		}
	}

	// If we got here, the objects have collided, we will displace r1
	// by overlap along the vector between the two object centers
	Vector2D d = { r2.pos.x - r1.pos.x, r2.pos.y - r1.pos.y };
	float s = sqrtf(d.x * d.x + d.y * d.y);
	r1.pos.x -= overlap * d.x / s;
	r1.pos.y -= overlap * d.y / s;
	return false;
}
*/

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

	myobj1 = new DGameObject("1x dirt", { 264, 248 }, "resources/dirt.png");
	myobj2 = new DGameObject("2x dirt", { 280, 248 }, "resources/dirt.png");
	myobj3 = new DGameObject("3x dirt", { 296, 248 }, "resources/dirt.png");

	myobj4 = new DGameObject("-1x dirt", { 232, 248 }, "resources/dirt.png");
	myobj5 = new DGameObject("-2x dirt", { 216, 248 }, "resources/dirt.png");
	myobj6 = new DGameObject("-3x dirt", { 200, 248 }, "resources/dirt.png");

	myobj7 = new DGameObject("0 dirt", { 248, 248 }, "resources/dirt.png");

	myobj8 = new DGameObject("1y dirt", { 248, 264 }, "resources/dirt.png");
	myobj9 = new DGameObject("2y dirt", { 248, 280 }, "resources/dirt.png");
	myobj10 = new DGameObject("3y dirt", { 248, 296 }, "resources/dirt.png");

	myobj11 = new DGameObject("-1y dirt", { 248, 232 }, "resources/dirt.png");
	myobj12 = new DGameObject("-2y dirt", { 248, 216 }, "resources/dirt.png");
	myobj13 = new DGameObject("-3y dirt", { 248, 200 }, "resources/dirt.png");

	GameWorld::customObject.push_back(myobj1);
	GameWorld::customObject.push_back(myobj2);
	GameWorld::customObject.push_back(myobj3);
	GameWorld::customObject.push_back(myobj4);
	GameWorld::customObject.push_back(myobj5);
	GameWorld::customObject.push_back(myobj6);
	GameWorld::customObject.push_back(myobj7);
	GameWorld::customObject.push_back(myobj8);
	GameWorld::customObject.push_back(myobj9);
	GameWorld::customObject.push_back(myobj10);
	GameWorld::customObject.push_back(myobj11);
	GameWorld::customObject.push_back(myobj12);
	GameWorld::customObject.push_back(myobj13);

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

	player->draw();
	myobj1->draw();
	myobj2->draw();
	myobj3->draw();
	myobj4->draw();
	myobj5->draw();
	myobj6->draw();
	myobj7->draw();
	myobj8->draw();
	myobj9->draw();
	myobj10->draw();
	myobj11->draw();
	myobj12->draw();
	myobj13->draw();

	GameWorld::getCurrentLevel()->drawLevel();

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
