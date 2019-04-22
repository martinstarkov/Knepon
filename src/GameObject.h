#pragma once
#include "Rectangle.h"
#include "Vector2D.h"
#include "Hitbox.h"

class GameObject {
public:
	SDL_Rect object;
	Hitbox hitbox;
	GameObject(std::string name, std::string texturePath, SDL_Rect rectangle, bool hasGravity);
	~GameObject();
	std::string id, texture;
	bool falling = false, jumping = false;
	Vector2D velocity = { 0, 0 };
	void updatePosition();
};

