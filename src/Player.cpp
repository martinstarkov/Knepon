#include "common.h"

Player* Player::instance;
std::vector<GameObject*> Player::collideableObjects;

Player::Player(std::string aName, Vector2D aPosition, std::string aTexture, Vector2D aSize, bool isInteractable, std::string aType, Vector2D aSpeed, SDL_RendererFlip aDirection, Vector2D aVelocity) : DUGameObject(aName, aPosition, aTexture, aSize, isInteractable, aType, aSpeed, aDirection, aVelocity), UGameObject(aName, aPosition, aSize, isInteractable, aType), DGameObject(aName, aPosition, aTexture, aSize, isInteractable, aDirection), GameObject(aName, aPosition, aSize, isInteractable) {
}

void Player::update(double dt) {
	if (!onGround) {
		velocity.y += gravity; //-(gravity * pow(dt, 2)) / 2;//
	}
	DUGameObject::update(dt);
}

void Player::move(MovementDirections movementDirection) {
	switch (movementDirection) {
		case UP:
			if (!jumping) {
				jumping = true;
				velocity.y = -1 * speed.y;
			}
			break;
		case DOWN:
			//possibly add downward movement control in the future?
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
	}
}

Player::~Player() {
	
}