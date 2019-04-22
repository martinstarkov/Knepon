#include "common.h"

GameObject::GameObject(std::string name, std::string texturePath, SDL_Rect objectRectangle, bool hasGravity, SDL_RendererFlip dir) : hitbox({ objectRectangle.x, objectRectangle.y, objectRectangle.w, objectRectangle.h }) {
	id = name;
	texture = texturePath;
	rectangle = { objectRectangle.x, objectRectangle.y, objectRectangle.w, objectRectangle.h };
	falling = hasGravity;
	direction = dir;
	Game::getInstance()->gameObjects.push_back(this);
	if (TextureManager::getInstance()->textureMap.count(id) == 0) {
		TextureManager::getInstance()->load(texture, id, Game::getInstance()->Game::getRenderer());
	}
}

GameObject::~GameObject() {
}

void GameObject::move(MovementDirections dir, Vector2D speed) {
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
			if (!jumping) {
				jumping = true;
				velocity.y += -speed.y;
			}
			break;
		}
		case DOWN: {
			velocity.y = speed.x;
			break;
		}
	}
}

void GameObject::updatePosition() {
	rectangle.x += velocity.x;
	rectangle.y += velocity.y;
}