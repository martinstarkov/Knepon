#pragma once

class Player : public GameObject {
public:
	Player(std::string name, std::string texturePath, SDL_Rect objectRectangle, bool hasGravity, SDL_RendererFlip dir);
	~Player();
	static Player* getInstance() {
		if (instance == 0) {
			return nullptr;
		}
		return instance;
	}

	double animationPhase = 0;
	int phases = 4;
private:
	static Player* instance;
};