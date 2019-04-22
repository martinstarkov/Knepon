#include "common.h"

GameObject::GameObject(std::string name, std::string texturePath, Rectangle objectRectangle, bool hasGravity) : hitbox({ objectRectangle.x, objectRectangle.y, objectRectangle.w, objectRectangle.h }) {
	id = name;
	texture = texturePath;
	rectangle.x = position.x = objectRectangle.x;
	rectangle.y = position.y = objectRectangle.y;
	rectangle.w = objectRectangle.w;
	rectangle.h = objectRectangle.h;
	falling = hasGravity;
	Game::getInstance()->gameObjects.push_back(this);
	TextureManager::getInstance()->load(texture, id, Game::getInstance()->Game::getRenderer());
}

void GameObject::updateNewPosition() {
	hitbox.min.x = rectangle.x;
	hitbox.min.y = rectangle.y + rectangle.h;
	hitbox.max.x = rectangle.x + rectangle.w;
	hitbox.max.y = rectangle.y;
}

void GameObject::updateOldPosition() {
	hitbox.oldMin.x = hitbox.min.x;
	hitbox.oldMax.x = hitbox.max.x;
	hitbox.oldMax.y = hitbox.max.y;
	hitbox.oldMin.y = hitbox.min.y;
}

void GameObject::updatePosition() {
	//tile scrolling
	rectangle.x = position.x - Player::getInstance()->camera.x;
	rectangle.y = position.y - Player::getInstance()->camera.y;
	updateOldPosition();
	updateNewPosition();
}

GameObject::~GameObject() {
}
