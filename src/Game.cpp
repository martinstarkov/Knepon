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

	myobj1 = new DGameObject("top dirt", { 250, 250 }, "resources/dirt.png");
	myobj2 = new DGameObject("corner dirt", { 250, 266 }, "resources/dirt.png");
	myobj3 = new DGameObject("bottom dirt", { 234, 266 }, "resources/dirt.png");
	GameWorld::customObject.push_back(myobj1);
	GameWorld::customObject.push_back(myobj2);
	GameWorld::customObject.push_back(myobj3);

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


	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_Rect rect = { (int)floor(DUGameObject::ptr->position.x), (int)floor(DUGameObject::ptr->position.y), (int)floor(DUGameObject::ptr->size.x), (int)floor(DUGameObject::ptr->size.y) };
	SDL_RenderDrawRect(renderer, &rect);

	//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	//SDL_RenderDrawPoint(renderer, int(DUGameObject::point->x), int(DUGameObject::point->y));

	

	player->draw();
	myobj1->draw();
	myobj2->draw();
	myobj3->draw();

	GameWorld::getCurrentLevel()->drawLevel();
	/*
	for (auto object : Player::getInstance()->collideableObjects) {
		if (object->getOverlap()) {
			SDL_SetRenderDrawColor(Game::getRenderer(), 0, 255, 0, 255);
		} else {
			SDL_SetRenderDrawColor(Game::getRenderer(), 255, 0, 0, 255);
		}
		SDL_Rect rect = { int(object->getPosition().x), int(object->getPosition().y), int(object->getSize().x), int(object->getSize().y) };
		SDL_RenderDrawRect(renderer, &rect);
	}
	*/
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
