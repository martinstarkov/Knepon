#include "common.h"

Player* Player::instance = 0; 

Player::Player(std::string name, std::string texturePath, SDL_Rect objectRectangle, bool hasGravity, SDL_RendererFlip dir) : GameObject(name, texturePath, objectRectangle, hasGravity, dir) {
	velocity = { 0, 0 };
	speed = { 2, 2 };
}

void Player::move(MovementDirections dir, Vector2D speed) {
	switch (dir) {
		case RIGHT: {
			velocity.x = speed.x;
			direction = SDL_FLIP_HORIZONTAL;
			break;
		}
		case LEFT: {
			velocity.x = -speed.x;
			direction = SDL_FLIP_NONE;
			break;
		}
		case UP: {
			velocity.y = -speed.y;
			break;
		}
		case DOWN: {
			velocity.y = speed.x;
			break;
		}
	}
}

void Player::updatePosition() {

	//make everything into functions and fix corner tunneling bug

	rectangle.x += velocity.x;
	rectangle.y += velocity.y;
	hitbox.oldMin.x = hitbox.min.x;
	hitbox.oldMin.y = hitbox.min.y;
	hitbox.oldMax.x = hitbox.max.x;
	hitbox.oldMax.y = hitbox.max.y;

	hitbox.min.x = rectangle.x;
	hitbox.max.y = rectangle.y;
	hitbox.max.x = rectangle.x + rectangle.w;
	hitbox.min.y = rectangle.y + rectangle.h;

	collidingGrid.clear();

	Vector2D tile = { floor((rectangle.x + rectangle.w / 2) / TILE_WIDTH), floor((rectangle.y + rectangle.h / 2) / TILE_HEIGHT) };

	SDL_Rect rect = { tile.x * 16, tile.y * 16, TILE_HEIGHT, TILE_WIDTH };
	collidingGrid.push_back(rect);
	bool right;
	if ((tile.x * 16 + TILE_WIDTH / 2) - (rectangle.x + rectangle.w / 2) >= 0) {
		rect = { (tile.x - 1) * 16, tile.y * 16, TILE_HEIGHT, TILE_WIDTH };
		right = false;
	} else {
		rect = { (tile.x + 1) * 16, tile.y * 16, TILE_HEIGHT, TILE_WIDTH };
		right = true;
	}
	collidingGrid.push_back(rect);
	if ((tile.y * 16 + TILE_HEIGHT / 2) - (rectangle.y + rectangle.h / 2) >= 0) {
		if (right) {
			rect = { (tile.x + 1) * 16, (tile.y - 1) * 16, TILE_HEIGHT, TILE_WIDTH };
		} else {
			rect = { (tile.x - 1) * 16, (tile.y - 1) * 16, TILE_HEIGHT, TILE_WIDTH };
		}
		collidingGrid.push_back(rect);
		rect = { tile.x * 16, (tile.y - 1) * 16, TILE_HEIGHT, TILE_WIDTH };
	} else {
		if (right) {
			rect = { (tile.x + 1) * 16, (tile.y + 1) * 16, TILE_HEIGHT, TILE_WIDTH };
		} else {
			rect = { (tile.x - 1) * 16, (tile.y + 1) * 16, TILE_HEIGHT, TILE_WIDTH };
		}
		collidingGrid.push_back(rect);
		rect = { tile.x * 16, (tile.y + 1) * 16, TILE_HEIGHT, TILE_WIDTH };
	}
	collidingGrid.push_back(rect);

}

Player::~Player() {
	TextureManager::getInstance()->TextureManager::clearFromTextureMap(id);
}