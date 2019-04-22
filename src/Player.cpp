#include "common.h"

Player* Player::instance = 0; 

Player::Player(std::string name, std::string texturePath, SDL_Rect objectRectangle, bool hasGravity, SDL_RendererFlip dir) : GameObject(name, texturePath, objectRectangle, hasGravity, dir) {
}

Player::~Player() {
	TextureManager::getInstance()->TextureManager::clearFromTextureMap(id);
}