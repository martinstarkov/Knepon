#pragma once

enum MovementDirections {
	UP = SDL_SCANCODE_W,
	LEFT = SDL_SCANCODE_A,
	DOWN = SDL_SCANCODE_S,
	RIGHT = SDL_SCANCODE_D,
};

class InputHandler {
public:
	static InputHandler* getInstance() {
		if (instance == 0) {
			instance = new InputHandler();
		}
		return instance;
	}
	void inputKeyStates(const Uint8* states);
	void processInput(SDL_Event* event);
private:
	static InputHandler* instance;
};

