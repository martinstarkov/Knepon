#include "common.h"

InputHandler* InputHandler::instance = 0;

void InputHandler::inputKeyStates(const Uint8* states) {
	if (states[LEFT] && !states[RIGHT]) {
		Player::getInstance()->move(LEFT);
	} else if (states[RIGHT] && !states[LEFT]) {
		Player::getInstance()->move(RIGHT);
	}
	if (!states[LEFT] && !states[RIGHT]) {
		Player::getInstance()->stop(X);
	}
	if (states[UP] && !states[DOWN]) {
		Player::getInstance()->move(UP);
	} else if (states[DOWN] && !states[UP]) {
		Player::getInstance()->move(DOWN);
	}
	if (!states[UP] && !states[DOWN]) {
		Player::getInstance()->stop(Y);
	}
	//reset position
	if (states[SDL_SCANCODE_R]) {
		Player::getInstance()->setPosition(Player::getInstance()->getOriginalPosition());
	}
}

void InputHandler::processInput(SDL_Event* event) {
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	inputKeyStates(keystates);
	while (SDL_PollEvent(event)) {
		switch ((*event).type) {
			case QUIT: {
				Game::getInstance()->quit();
				break;
			}
			default: {
				break;
			}
		}
	}
}
