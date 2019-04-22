#include "common.h"
#define up 0
#define left 1
#define down 2
#define right 3

InputHandler* InputHandler::instance = 0;

void InputHandler::animationPhase() {
	if (Player::getInstance()->animationPhase < (Player::getInstance()->phases - 1)) {
		Player::getInstance()->animationPhase += 0.3;
	} else {
		Player::getInstance()->animationPhase = 0;
	}
}

void InputHandler::resetAnimation() {
	Player::getInstance()->animationPhase = 0;
}

void InputHandler::inputKeyStates(const Uint8* states) {
	if (states[SDL_SCANCODE_LEFT] || states[SDL_SCANCODE_RIGHT]) {
		InputHandler::getInstance()->animationPhase();
	}
	//figure out states by making "was pressed down" variable which indicicates the state of keys in the previous frame
	//and CLEAN UP CODE
	if (states[SDL_SCANCODE_LEFT]) {
		Player::getInstance()->move(left);
	}
	if (states[SDL_SCANCODE_RIGHT]) {
		Player::getInstance()->move(right);
	}
	if (states[SDL_SCANCODE_UP]) {
		Player::getInstance()->move(up);
	}
	if (states[SDL_SCANCODE_DOWN]) {
		Player::getInstance()->move(down);
	}
	/* Check definition of zombie code; this platformer has gravity ;)
	if (states[SDL_SCANCODE_UP] == 0 && Player::getInstance()->velocity.y < 0) {
		Player::getInstance()->stopMoving("y");
	}
	if (states[SDL_SCANCODE_DOWN] == 0 && Player::getInstance()->velocity.y > 0) {
		Player::getInstance()->stopMoving("y");
	}
	*/
	if (states[SDL_SCANCODE_LEFT] == 0 && Player::getInstance()->velocity.x < 0) {
		Player::getInstance()->stopMoving("x");
	}
	if (states[SDL_SCANCODE_RIGHT] == 0 && Player::getInstance()->velocity.x > 0) {
		Player::getInstance()->stopMoving("x");
	}
}

void InputHandler::processInput(SDL_Event event) {
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	inputKeyStates(keystates);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: {
			Game::getInstance()->quit();
			break;
		}
		default:
			break;
		}
	}
}
