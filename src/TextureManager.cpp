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

void TextureManager::draw(std::string id, Rectangle rectangle, SDL_Renderer* renderer, SDL_RendererFlip flip) {
	SDL_Rect srcRect, destRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = (int)rectangle.w;
	srcRect.h = destRect.h = (int)rectangle.h;
	destRect.x = (int)rectangle.x;
	destRect.y = (int)rectangle.y;
	SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect, NULL, NULL, flip);
}

void TextureManager::animate(std::string id, double phase, Rectangle rectangle, SDL_Renderer* renderer, SDL_RendererFlip flip) {
	SDL_Rect src, dest;
	src.y = 0;
	src.x = (int)rectangle.w * int(floor (phase));
	src.w = (int)rectangle.w;
	src.h = (int)rectangle.h;
	dest.x = (int)rectangle.x;
	dest.y = (int)rectangle.y;
	dest.w = (int)rectangle.w;
	dest.h = (int)rectangle.h;
	SDL_RenderCopyEx(renderer, textureMap[id], &src, &dest, NULL, NULL, flip);
}

void TextureManager::clearFromTextureMap(std::string id) {
	textureMap.erase(id);
}