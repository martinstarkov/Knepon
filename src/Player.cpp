#include "common.h"

Player* Player::instance = 0; 

Player::Player(std::string name, std::string texturePath, SDL_Rect objectRectangle, bool hasGravity, SDL_RendererFlip dir) : GameObject(name, texturePath, objectRectangle, hasGravity, dir) {
	velocity = { 0, 0 };
	speed = { 2, 2 };
}

void Player::move(MovementDirections dir, Vector2D speed) {
	switch (dir) {
		case RIGHT: {
			if (canMoveRight) {
				velocity.x = speed.x;
				direction = SDL_FLIP_HORIZONTAL;
			}
			break;
		}
		case LEFT: {
			if (canMoveLeft) {
				velocity.x = -speed.x;
				direction = SDL_FLIP_NONE;
			}
			break;
		}
		case UP: {
			if (canMoveUp) {
				velocity.y = -speed.y;
			}
			break;
		}
		case DOWN: {
			if (canMoveDown) {
				velocity.y = speed.x;
			}
			break;
		}
	}
}

void Player::updatePosition() {

	//make everything into functions and fix corner tunneling bug

	rectangle.x += velocity.x;
	hitbox.oldMin.x = hitbox.min.x;
	hitbox.oldMax.x = hitbox.max.x;
	hitbox.min.x = rectangle.x;
	hitbox.max.x = rectangle.x + rectangle.w;

	rectangle.y += velocity.y;
	hitbox.oldMin.y = hitbox.min.y;
	hitbox.oldMax.y = hitbox.max.y;
	hitbox.max.y = rectangle.y;
	hitbox.min.y = rectangle.y + rectangle.h;

	rightCollider = nullptr;
	leftCollider = nullptr;
	topCollider = nullptr;
	bottomCollider = nullptr;
	canMoveRight = true;
	canMoveLeft = true;
	canMoveDown = true;
	canMoveUp = true;

	for (auto& object : Game::getInstance()->gameObjects) {
		if (hitbox.max.x == object->hitbox.min.x || hitbox.max.x == LEVEL_WIDTH) {
			if (collides(object) || hitbox.max.x == LEVEL_WIDTH) {
				canMoveRight = false;
				if (hitbox.max.x != LEVEL_WIDTH) {
					rightCollider = object;
				} else {
					rightCollider = nullptr;
				}
			}
		}
		if (hitbox.min.x == object->hitbox.max.x || hitbox.min.x == 0) {
			if (collides(object) || hitbox.min.x == 0) {
				canMoveLeft = false;
				if (hitbox.min.x != 0) {
					leftCollider = object;
				} else {
					leftCollider = nullptr;
				}
			}
		}
		if (hitbox.max.y == object->hitbox.min.y || hitbox.max.y == 0) {
			if (collides(object) || hitbox.max.y == 0) {
				canMoveUp = false;
				if (hitbox.max.y != 0) {
					topCollider = object;
				} else {
					topCollider = nullptr;
				}
			}
		}
		if (hitbox.min.y == object->hitbox.max.y || hitbox.min.y == LEVEL_HEIGHT) {
			if (collides(object) || hitbox.min.y == LEVEL_HEIGHT) {
				canMoveDown = false;
				if (hitbox.min.y != LEVEL_HEIGHT) {
					bottomCollider = object;
				}
			}
		}
	}

	if (!canMoveRight) {
		if (rightCollider != nullptr) {
			rectangle.x = rightCollider->hitbox.min.x - rectangle.w;
			hitbox.min.x = rightCollider->hitbox.min.x - rectangle.w;
			hitbox.max.x = rightCollider->hitbox.min.x;
		} else {
			rectangle.x = LEVEL_WIDTH - rectangle.w;
			hitbox.min.x = LEVEL_WIDTH - rectangle.w;
			hitbox.max.x = LEVEL_WIDTH;
		}
		velocity.x = 0;
	}

	if (!canMoveLeft) {
		if (leftCollider != nullptr) {
			rectangle.x = leftCollider->hitbox.max.x;
			hitbox.min.x = leftCollider->hitbox.max.x;
			hitbox.max.x = leftCollider->hitbox.min.x + rectangle.w;
		} else {
			rectangle.x = 0;
			hitbox.min.x = 0;
			hitbox.max.x = rectangle.w;
		}
		velocity.x = 0;
	}

	if (!canMoveUp) {
		if (topCollider != nullptr) {
			rectangle.y = topCollider->hitbox.min.y;
			hitbox.max.y = topCollider->hitbox.min.y;
			hitbox.min.y = topCollider->hitbox.min.x + rectangle.h;
		} else {
			rectangle.y = 0;
			hitbox.max.y = 0;
			hitbox.min.y = rectangle.h;
		}
		velocity.y = 0;
	}

	if (!canMoveDown) {
		if (bottomCollider != nullptr) {
			rectangle.y = bottomCollider->hitbox.max.y - rectangle.h;
			hitbox.max.y = bottomCollider->hitbox.max.y - rectangle.h;
			hitbox.min.y = bottomCollider->hitbox.max.y;
		} else {
			rectangle.y = LEVEL_HEIGHT - rectangle.h;
			hitbox.max.y = LEVEL_HEIGHT - rectangle.h;
			hitbox.min.y = LEVEL_HEIGHT;
		}
		velocity.y = 0;
	}
	
}

Player::~Player() {
	TextureManager::getInstance()->TextureManager::clearFromTextureMap(id);
}