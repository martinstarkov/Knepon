#pragma once
#include "Vector2D.h"
#include "Hitbox.h"
#include "InputHandler.h"

class GameObject {
public:
	GameObject(std::string name, std::string texturePath, SDL_Rect objectRectangle, bool hasGravity, SDL_RendererFlip dir);
	~GameObject();
	bool GameObject::collides(GameObject* object);
	virtual void updatePosition();
	void updateHitbox();

	SDL_Rect rectangle;
	Hitbox hitbox;
	std::string id;
	bool falling = false;
	Vector2D velocity = { 0, 0 }, speed = { 0, 0 };
	SDL_RendererFlip direction;
private:
	std::string texture;
};

