#include "common.h"

TextureManager* TextureManager::instance = 0;

bool TextureManager::load(std::string filePath, std::string id, SDL_Renderer* renderer) {
	SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
	if (tempSurface == 0) {
		std::cout << "Could not load image with path: '" << filePath << "'!" << std::endl;
		return false;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	if (texture != 0) {
		std::cout << "Texture with path ('" << filePath << "') loaded succesfully!" << std::endl;
		textureMap[id] = texture;
		return true;
	}
	std::cout << "Texture loading failed for whatever reason." << std::endl;
	return false;
}

void TextureManager::draw(std::string id, SDL_Rect rectangle, SDL_Renderer* renderer, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = rectangle.w;
	srcRect.h = rectangle.h;
	SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &rectangle, NULL, NULL, flip);
}

void TextureManager::drawAnimated(std::string id, int phase, SDL_Rect rectangle, SDL_Renderer* renderer, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	srcRect.y = 0;
	srcRect.x = rectangle.w * floor(phase);
	srcRect.w = rectangle.w;
	srcRect.h = rectangle.h;
	SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &rectangle, NULL, NULL, flip);
}

void TextureManager::clearFromTextureMap(std::string id) {
	textureMap.erase(id);
}