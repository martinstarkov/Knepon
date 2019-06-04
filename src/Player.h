#pragma once
#include "InputHandler.h"

extern const int SCREEN_WIDTH, SCREEN_HEIGHT, LEVEL_WIDTH, LEVEL_HEIGHT;

class Player : public DUGameObject {
public:
	Player(std::string aName = "player", Vector2D aPosition = { 0, 0}, std::string aType = "player", std::string aTexture = "resources/squirrel_resting.png", Vector2D playerSpeed = { 0, 0 }, SDL_RendererFlip aDirection = SDL_FLIP_NONE, Vector2D initVelocity = { 0, 0 });
	~Player();
	static Player* getInstance() {
		if (instance == 0) {
			instance = new Player("player", { 140, 250 }, "player", "resources/squirrel_resting.png", { 150, 600 });
		}
		return instance;
	}
	void update(double dt) override;
	void move(MovementDirections movementDirection);
	void stop(Axis axis);
	std::vector<Vector2D> collisionRadius;
	static std::vector<GameObject*> collideableObjects;

	double gravity = 400;
private:
	static Player* instance;
};