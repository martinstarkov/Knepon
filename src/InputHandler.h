#pragma once

class InputHandler {
public:
	static InputHandler* getInstance() {
		if (instance == 0) {
			instance = new InputHandler();
		}
		return instance;
	}
	void resetAnimation();
	void animationPhase();
	void inputKeyStates(const Uint8* states);
	void processInput(SDL_Event event);
private:
	static InputHandler* instance;
};

