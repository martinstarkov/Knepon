#pragma once

extern const int SCREEN_WIDTH, SCREEN_HEIGHT, LEVEL_WIDTH, LEVEL_HEIGHT;

class Player : public GameObject {
public:
	Player(std::string name, std::string texturePath, SDL_Rect objectRectangle, bool hasGravity, SDL_RendererFlip dir);
	~Player();
	static Player* getInstance() {
		if (instance == 0) {
			instance = new Player("Player", "resources/squirrel_resting.png", { SCREEN_WIDTH / 2 - 8, SCREEN_HEIGHT / 2 - 8, 16, 16 }, true, SDL_FLIP_NONE);
		}
		return instance;
	}
	void move(MovementDirections dir, Vector2D speed);
	virtual void updatePosition();

	double animationPhase = 0;
	bool jumping = false;
	bool canMoveRight = true, canMoveLeft = true, canMoveUp = true, canMoveDown = true;
	GameObject* rightCollider = nullptr, * leftCollider = nullptr, * topCollider = nullptr, * bottomCollider = nullptr;
	int phases = 4;
private:
	static Player* instance;
};