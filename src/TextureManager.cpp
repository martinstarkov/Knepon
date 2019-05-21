#include "common.h"

TextureManager* TextureManager::instance;
std::map<std::string, SDL_Texture*> TextureManager::textureMap;

bool TextureManager::load(std::string filePath) {
	SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
	if (tempSurface == 0) {
		std::cout << "Could not load image with path: '" << filePath << "'!" << std::endl;
		return false;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::getRenderer(), tempSurface);
	SDL_FreeSurface(tempSurface);
	if (texture != 0) {
		if (textureMap.find(filePath) == textureMap.end()) {
			std::cout << "Texture with path ('" << filePath << "') loaded succesfully!" << std::endl;
			textureMap[filePath] = texture;
		} else {
			//std::cout << "Texture with path ('" << filePath << "') already loaded." << std::endl;
		}
		return true;
	}
	std::cout << "Texture loading failed for whatever reason." << std::endl;
	return false;
}

void TextureManager::draw(std::string texturePath, SDL_Rect rectangle, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = rectangle.w;
	srcRect.h = rectangle.h;
	SDL_RenderCopyEx(Game::getRenderer(), textureMap[texturePath], &srcRect, &rectangle, NULL, NULL, flip);
}

void TextureManager::clearFromTextureMap(std::string id) {
	textureMap.erase(id);
}