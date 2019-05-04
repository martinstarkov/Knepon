#include "common.h"

GameObject::GameObject(std::string name, std::string texturePath, SDL_Rect objectRectangle, bool hasGravity, SDL_RendererFlip dir) : hitbox({ objectRectangle.x, objectRectangle.y, objectRectangle.w, objectRectangle.h }) {
	id = name;
	texture = texturePath;
	rectangle = { objectRectangle.x, objectRectangle.y, objectRectangle.w, objectRectangle.h };
	falling = hasGravity;
	direction = dir;
	if (name != "Player") {
		Game::getInstance()->gameObjects.push_back(this);
	}
	if (TextureManager::getInstance()->textureMap.count(id) == 0) {
		TextureManager::getInstance()->load(texture, id, Game::getInstance()->Game::getRenderer());
	}
}

GameObject::~GameObject() {
}

bool GameObject::collides(GameObject* object) {
	Hitbox hb = object->hitbox;
	if (hitbox.min.y < hb.max.y || hitbox.max.y > hb.min.y || hitbox.min.x > hb.max.x || hitbox.max.x < hb.min.x) {
		//no collision
		return false;
	}
	if ((hitbox.max.x == hb.min.x && hitbox.min.y == hb.max.y) || (hitbox.min.x == hb.max.x && hitbox.min.y == hb.max.y) || (hitbox.min.x == hb.max.x && hitbox.max.y == hb.min.y) || (hitbox.max.x == hb.min.x && hitbox.max.y == hb.min.y)) {
		//corner collisions don't count
		return false;
	}
	//collision exists!
	return true;
}

void GameObject::updateHitbox() {
	hitbox.oldMin.x = hitbox.min.x;
	hitbox.oldMin.y = hitbox.min.y;
	hitbox.oldMax.x = hitbox.max.x;
	hitbox.oldMax.y = hitbox.max.y;

	hitbox.min.x = rectangle.x;
	hitbox.max.y = rectangle.y;
	hitbox.max.x = rectangle.x + rectangle.w;
	hitbox.min.y = rectangle.y + rectangle.h;
}

void GameObject::updatePosition() {
	rectangle.x += velocity.x;
	rectangle.y += velocity.y;
	updateHitbox();
}