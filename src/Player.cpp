#include "common.h"
#define up 0
#define left 1
#define down 2
#define right 3

Player* Player::instance = 0; 

Player::Player(std::string name, std::string texturePath, Rectangle playerRectangle, SDL_RendererFlip dir) : hitbox({ playerRectangle.x, playerRectangle.y, playerRectangle.w, playerRectangle.h }) {
	id = name;
	texture = texturePath;
	player.x = playerRectangle.x;
	player.y = playerRectangle.y;
	player.w = playerRectangle.w;
	player.h = playerRectangle.h;
	direction = dir;
	TextureManager::getInstance()->load(texture, id, Game::getInstance()->Game::getRenderer());
}

Player::~Player() {
	TextureManager::getInstance()->TextureManager::clearFromTextureMap(id);
}

void Player::updateCameraPosition() {
	camera.x = (player.x + player.w / 2) - SCREEN_WIDTH / 2;
	camera.y = (player.y + player.h / 2) - SCREEN_HEIGHT / 2;
	if (camera.x < 0) {
		camera.x = 0;
	}
	if (camera.y < 0) {
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w) {
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Player::updateOldPosition() {
	hitbox.oldMin.x = hitbox.min.x;
	hitbox.oldMax.x = hitbox.max.x;
	hitbox.oldMax.y = hitbox.max.y;
	hitbox.oldMin.y = hitbox.min.y;
}

void Player::updateNewXPosition() {
	hitbox.min.x = player.x;
	hitbox.max.x = player.x + player.w;
}

void Player::updateNewYPosition() {
	hitbox.min.y = player.y + player.h;
	hitbox.max.y = player.y;
}

std::string Player::collisionX(GameObject* objectP) {
	Hitbox object = objectP->hitbox;
	if (hitbox.min.y < object.max.y || hitbox.max.y > object.min.y || hitbox.min.x > object.max.x || hitbox.max.x < object.min.x) {
		return "no";
	} else {
		if (hitbox.max.x >= object.min.x && hitbox.oldMax.x < object.oldMin.x) {
			return "left";
		} else if (hitbox.min.x <= object.max.x && hitbox.oldMin.x > object.oldMax.x) {
			return "right";
		}
	}
	return "no";
}

std::string Player::collisionY(GameObject* objectP) {
	Hitbox object = objectP->hitbox;
	if (hitbox.min.y < object.max.y || hitbox.max.y > object.min.y || hitbox.min.x > object.max.x || hitbox.max.x < object.min.x) {
		return "no";
		InputHandler::getInstance()->resetAnimation();
	} else {
		if (hitbox.min.y >= object.max.y && hitbox.oldMin.y < object.oldMax.y) {
			return "top";
		} else if (hitbox.max.y <= object.min.y && hitbox.oldMax.y > object.oldMin.y) {
			return "bottom";
		}
	}
	return "no";
}


//make gameObjects array = presentObjects array so when something is not on screen it will be removed from collision checks

void Player::resolveXCollisions() {
	for (auto i : Game::getInstance()->gameObjects) {
		if (collisionX(i) != "no") {
			//std::cout << "collided with " << collisionX(i) << " side of " << i->id << std::endl;
			if (collisionX(i) == "left") {
				player.x = i->hitbox.min.x - player.w - 0.00000001;
				updateNewXPosition();
				velocity.x = 0;
			} else if (collisionX(i) == "right") {
				player.x = i->hitbox.max.x + 0.00000001;
				updateNewXPosition();
				velocity.x = 0;
			}
		}
	}
}

void Player::resolveYCollisions() {
	for (auto i : Game::getInstance()->gameObjects) {
		if (collisionY(i) != "no") {
			//std::cout << "collided with " << collisionY(i) << " side of " << i->id << std::endl;
			if (collisionY(i) == "top") {
				topCollisions++;
				player.y = i->hitbox.max.y - player.h - 0.00000001;
				updateNewYPosition();
				velocity.y = 0;
			} else if (collisionY(i) == "bottom") {
				bottomCollisions++;
				player.y = i->hitbox.min.y + 0.00000001;
				updateNewYPosition();
				velocity.y = 0;
			}
		}
	}
	if (topCollisions == 0 && bottomCollisions == 0) {
		falling = true;
		jumping = true;
	} else if (topCollisions > 0) {
		falling = false;
		jumping = false;
	}
	topCollisions = 0;
	bottomCollisions = 0;
}

void Player::move(int dir) {
	switch (dir) {
		case right: {
			velocity.x = speed.x;
			direction = SDL_FLIP_HORIZONTAL;
			break;
		}
		case left: {
			velocity.x = -speed.x;
			direction = SDL_FLIP_NONE;
			break;
		}
		case up:
			if (!jumping) {
				jumping = true;
				velocity.y += -speed.y;
			}
			break;
		case down: {
			//using the same vertical speed as horizontal for now 
			//velocity.y = speed.x;
			break;
		}
		default: {
			break;
		}
	}
}

void Player::stopMoving(std::string axis) {
	if (axis == "x") {
		velocity.x = 0;
	} else if (axis == "y") {
		velocity.y = 0;
	}
}

void Player::updatePosition() {
	updateOldPosition();
	if (player.x + player.w + velocity.x > LEVEL_WIDTH) {
		player.x = LEVEL_WIDTH - player.w - 0.00000001;
		velocity.x = 0;
	} else if (player.x + velocity.x < 0) {
		player.x = 0.00000001;
		velocity.x = 0;
	} else {
		player.x += velocity.x;
	}
	updateNewXPosition();
	resolveXCollisions();
	if (falling) {
		InputHandler::getInstance()->resetAnimation();
		velocity.y += gravityConstant;
	}
	if (player.y + player.h + velocity.y > LEVEL_HEIGHT && falling) {
		player.y = LEVEL_HEIGHT - player.h;
		velocity.y = 0;
		topCollisions++;
	} else if (player.y + velocity.y < 0) {
		player.y = 0.00000001;
		velocity.y = 0;
	} else {
		player.y += velocity.y;
	}
	updateNewYPosition();
	resolveYCollisions();
	updateCameraPosition();
	//std::cout << "Difference: (x:" << abs(hitbox.oldMin.x - hitbox.min.x) << ", y:" << abs(hitbox.oldMax.y - hitbox.max.y) << ")" << std::endl;
	//std::cout << "Old: (" << hitbox.oldMin.x << "," << hitbox.oldMax.y << "), New: (" << hitbox.min.x << "," << hitbox.max.y << ")" << std::endl;
}