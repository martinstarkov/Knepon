#include "common.h"

Player* Player::instance;
std::vector<GameObject*> Player::collideableObjects;

Player::Player(std::string aName, Vector2D aPosition, std::string aType, std::string aTexture, Vector2D playerSpeed, SDL_RendererFlip aDirection, Vector2D initVelocity) : DUGameObject(aName, aPosition, aType, aTexture, playerSpeed, aDirection, initVelocity), UGameObject(aName, aPosition, aType), DGameObject(aName, aPosition, aTexture, aDirection), GameObject(aName, aPosition) {
}

void Player::update(double dt) {
	DUGameObject::update(dt);
}

void Player::move(MovementDirections movementDirection) {
	switch (movementDirection) {
		case UP:
			velocity.y = -1 * speed.y;
			break;
		case DOWN:
			velocity.y = 1 * speed.y;
			break;
		case LEFT:
			direction = SDL_FLIP_NONE;
			velocity.x = -1 * speed.x;
			break;
		case RIGHT:
			direction = SDL_FLIP_HORIZONTAL;
			velocity.x = 1 * speed.x;
			break;
	}
}

void Player::stop(Axis axis) {
	if (axis == X) {
		velocity.x = 0;
	} else if (axis == Y) {
		velocity.y = 0;
	}
}
/*
void Player::move(MovementDirections dir, Vector2D speed) {

}

void Player::updatePosition() {
	//make everything into functions

	//clean up colliding grid systems

	//find out way to find direction (side) of collision in order to implement perfect rectangle.x = object->hitbox.min.x; resolution

	collidingGrid.clear();
	xCollidingGrid.clear();
	yCollidingGrid.clear();
	int playerCoordinates[4][2] = { { hitbox.min.x, hitbox.max.y }, { hitbox.max.x, hitbox.max.y }, { hitbox.max.x, hitbox.min.y }, { hitbox.min.x, hitbox.min.y } };
	for (int i = 0; i < 4; i++) {
		Vector2D corner = { playerCoordinates[i][0], playerCoordinates[i][1] };
		Vector2D cornerTile = { floor(corner.x / TILE_WIDTH), floor(corner.y / TILE_HEIGHT) };
		collidingGrid.push_back(cornerTile);
		xCollidingGrid.push_back(cornerTile);
		yCollidingGrid.push_back(cornerTile);
		switch (i) {
			case 0: {
				collidingGrid.push_back({ cornerTile.x - 1, cornerTile.y });
				collidingGrid.push_back({ cornerTile.x, cornerTile.y - 1 });
				collidingGrid.push_back({ cornerTile.x - 1, cornerTile.y - 1 });

				xCollidingGrid.push_back({ cornerTile.x - 1, cornerTile.y });
				yCollidingGrid.push_back({ cornerTile.x, cornerTile.y - 1 });
				break;
			}
			case 1: {
				collidingGrid.push_back({ cornerTile.x + 1, cornerTile.y });
				collidingGrid.push_back({ cornerTile.x, cornerTile.y - 1 });
				collidingGrid.push_back({ cornerTile.x + 1, cornerTile.y - 1 });

				xCollidingGrid.push_back({ cornerTile.x + 1, cornerTile.y });
				yCollidingGrid.push_back({ cornerTile.x, cornerTile.y - 1 });
				break;
			}
			case 2: {
				collidingGrid.push_back({ cornerTile.x + 1, cornerTile.y });
				collidingGrid.push_back({ cornerTile.x, cornerTile.y + 1 });
				collidingGrid.push_back({ cornerTile.x + 1, cornerTile.y + 1 });

				xCollidingGrid.push_back({ cornerTile.x + 1, cornerTile.y });
				yCollidingGrid.push_back({ cornerTile.x, cornerTile.y + 1 });
				break;
			}
			case 3: {
				collidingGrid.push_back({ cornerTile.x - 1, cornerTile.y });
				collidingGrid.push_back({ cornerTile.x, cornerTile.y + 1 });
				collidingGrid.push_back({ cornerTile.x - 1, cornerTile.y + 1 });

				xCollidingGrid.push_back({ cornerTile.x - 1, cornerTile.y });
				yCollidingGrid.push_back({ cornerTile.x, cornerTile.y + 1 });
				break;
			}
		}
	}*/

	/*
	hitbox.oldMin.x = hitbox.min.x;
	hitbox.oldMax.x = hitbox.max.x;
	hitbox.min.x = rectangle.x;
	hitbox.max.x = rectangle.x + rectangle.w;

	int xdepth = 0, ydepth = 0;
	for (auto& tile : xCollidingGrid) {
		if (tile.y >= Game::getInstance()->currentLevel->mapObjects.size() || tile.x >= Game::getInstance()->currentLevel->mapObjects[0].size()) {
			continue;
		}
		collisionObject = Game::getInstance()->currentLevel->mapObjects[tile.y][tile.x];
		if (collisionObject != nullptr) {
			if (collides(collisionObject)) {
				xdepth = abs(collisionObject->hitbox.min.x + collisionObject->hitbox.size.x / 2) - (hitbox.min.x + rectangle.w / 2);
				ydepth = abs(collisionObject->hitbox.max.y + collisionObject->hitbox.size.y / 2) - (hitbox.max.y + rectangle.h / 2);
				if (xdepth < 0 && abs(xdepth) > abs(ydepth)) {
					std::cout << "left collision" << std::endl;
					rectangle.x = collisionObject->hitbox.max.x;
					hitbox.min.x = collisionObject->hitbox.max.x;
					hitbox.max.x = collisionObject->hitbox.max.x + rectangle.w;
					velocity.x = 0;
					//left collision
				} else if (xdepth >= 0 && abs(xdepth) > abs(ydepth)) {
					std::cout << "right collision" << std::endl;
					rectangle.x = collisionObject->hitbox.min.x - rectangle.w;
					hitbox.min.x = collisionObject->hitbox.min.x - rectangle.w;
					hitbox.max.x = collisionObject->hitbox.min.x;
					velocity.x = 0;
					//left collision
				}
			}
		}
	}
	*/
	//rectangle.x += int(velocity.x);
	//rectangle.y += int(velocity.y);

	/*
	hitbox.oldMin.y = hitbox.min.y;
	hitbox.oldMax.y = hitbox.max.y;
	hitbox.max.y = rectangle.y;
	hitbox.min.y = rectangle.y + rectangle.h;

	for (auto& tile : yCollidingGrid) {
		if (tile.y >= Game::getInstance()->currentLevel->mapObjects.size() || tile.x >= Game::getInstance()->currentLevel->mapObjects[0].size()) {
			continue;
		}
		collisionObject = Game::getInstance()->currentLevel->mapObjects[tile.y][tile.x];
		if (collisionObject != nullptr) {
			if (collides(collisionObject)) {
				xdepth = abs(collisionObject->hitbox.min.x + collisionObject->hitbox.size.x / 2) - (hitbox.min.x + rectangle.w / 2);
				ydepth = abs(collisionObject->hitbox.max.y + collisionObject->hitbox.size.y / 2) - (hitbox.max.y + rectangle.h / 2);
				if (ydepth < 0 && abs(ydepth) > abs(xdepth)) {
					std::cout << "top collision" << std::endl;
					rectangle.y = collisionObject->hitbox.min.y;
					hitbox.max.y = collisionObject->hitbox.min.y;
					hitbox.min.y = collisionObject->hitbox.min.y + rectangle.h;
					velocity.y = 0;
					//hitting head
				} else if (ydepth >= 0 && abs(ydepth) > abs(xdepth)) {
					std::cout << "bottom collision" << std::endl;
					rectangle.y = collisionObject->hitbox.max.y - rectangle.h;
					hitbox.max.y = collisionObject->hitbox.max.y - rectangle.h;
					hitbox.min.y = collisionObject->hitbox.max.y;
					velocity.y = 0;
					//landing on ground
				}
			}
		}
	}
}*/

Player::~Player() {
	//TextureManager::getInstance()->TextureManager::clearFromTextureMap(id);
}