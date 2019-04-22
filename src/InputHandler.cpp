#include "common.h"

InputHandler* InputHandler::instance = 0;

void InputHandler::inputKeyStates(const Uint8* states) {
	Player* player = Player::getInstance();
	if (states[LEFT]) {
		player->move(LEFT, player->speed);
	} else if (!states[LEFT]) {
		player->move(LEFT, { 0, 0 });
	}
	if (states[RIGHT]) {
		player->move(RIGHT, player->speed);
	} else if (!states[RIGHT]) {
		player->move(RIGHT, { 0, 0 });
	}
	if (states[UP]) {
		player->move(UP, player->speed);
	} else if (!states[UP]) {
		player->move(UP, { 0, 0 });
	}
	if (states[DOWN]) {
		player->move(DOWN, player->speed);
	} else if (!states[DOWN]) {
		player->move(DOWN, { 0, 0 });
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
