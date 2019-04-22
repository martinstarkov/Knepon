#pragma once

//gravity was 2
const double gravityConstant = 2.5;

extern const double SCREEN_WIDTH;
extern const double SCREEN_HEIGHT;
extern const double LEVEL_HEIGHT;
extern const double LEVEL_WIDTH;

class Player {
public:
	static Player* getInstance() {
		if (instance == 0) {
			instance = new Player("Player", "resources/slip.png", { SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 140, 280, 280 }, SDL_FLIP_NONE);
		}
		return instance;
	}
	void updatePosition();
	double animationPhase = 0;
	double phases = 4;
	double mass = 1;
	void move(int dir);
	void stopMoving(std::string axis);
	std::string collisionX(GameObject* objectP);
	std::string collisionY(GameObject* objectP);
	void resolveXCollisions();
	void resolveYCollisions();
	SDL_RendererFlip direction;
	void updateCameraPosition();
	void updateOldPosition();
	void updateNewXPosition();
	void updateNewYPosition();
	int topCollisions = 0;
	int bottomCollisions = 0;
	bool canMoveLeft = true, canMoveRight = true;
	bool falling = true, jumping = true;
	Vector2D velocity = { 0, 0 };
	Rectangle player;
	std::string id, texture;
	Player(std::string name, std::string texturePath, Rectangle playerRectangle, SDL_RendererFlip dir);
	~Player();
	Vector2D speed = { 5, 45 };
	Hitbox hitbox;
	static SDL_Rect camera;
private:
	static Player* instance;
};