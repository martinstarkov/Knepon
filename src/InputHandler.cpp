#include "common.h"

InputHandler* InputHandler::instance = 0;

void InputHandler::inputKeyStates(const Uint8* states) {
	if (states[LEFT] && states[RIGHT] == 0) {
		Player::getInstance()->move(LEFT, Player::getInstance()->speed);
	}
	if (states[RIGHT] && states[LEFT] == 0) {
		Player::getInstance()->move(RIGHT, Player::getInstance()->speed);
	}
	if (states[UP] && states[DOWN] == 0) {
		Player::getInstance()->move(UP, Player::getInstance()->speed);
	}
	if (states[DOWN] && states[UP] == 0) {
		Player::getInstance()->move(DOWN, Player::getInstance()->speed);
	}
	if (states[LEFT] == 0 && Player::getInstance()->velocity.x < 0) {
		Player::getInstance()->move(LEFT, { 0, 0 });
	}
	if (states[DOWN] == 0 && Player::getInstance()->velocity.y > 0) {
		Player::getInstance()->move(DOWN, { 0, 0 });
	}
	if (states[UP] == 0 && Player::getInstance()->velocity.y < 0) {
		Player::getInstance()->move(UP, { 0, 0 });
	}
	if (states[RIGHT] == 0 && Player::getInstance()->velocity.x > 0) {
		Player::getInstance()->move(RIGHT, { 0, 0 });
	}
}

void InputHandler::processInput(SDL_Event* event) {
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	inputKeyStates(keystates);
	while (SDL_PollEvent(event)) {
		switch ((*event).type) {
		case SDL_QUIT: {
			Game::getInstance()->quit();
			break;
		}
		default:
			break;
		}
	}
}
