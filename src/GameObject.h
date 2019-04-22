#pragma once
#include "Vector2D.h"
#include "Hitbox.h"
#include "InputHandler.h"

class GameObject {
public:
	GameObject(std::string name, std::string texturePath, SDL_Rect objectRectangle, bool hasGravity, SDL_RendererFlip dir);
	~GameObject();
	void move(MovementDirections dir, Vector2D speed);
	void updatePosition();

	SDL_Rect rectangle;
	Hitbox hitbox;
	std::string id, texture;
	bool falling = false, jumping = false;
	Vector2D velocity = { 0, 0 }, speed = { 5, 45 };
	SDL_RendererFlip direction;
};

