#pragma once

enum MovementDirections {
	QUIT = SDL_QUIT,
	UP = SDL_SCANCODE_W,
	LEFT = SDL_SCANCODE_A,
	DOWN = SDL_SCANCODE_S,
	RIGHT = SDL_SCANCODE_D,
};

enum Axis {
	X = true,
	Y = false
};

class InputHandler {
public:
	static InputHandler* getInstance() {
		if (instance == 0) {
			instance = new InputHandler();
		}
		return instance;
	}
	static void processInput(SDL_Event* event);
private:
	static void inputKeyStates(const Uint8* states);
	static InputHandler* instance;
};

