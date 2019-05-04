#pragma once

class TextureManager {
public:
	static TextureManager* getInstance() {
		if (instance == 0) {
			instance = new TextureManager();
		}
		return instance;
	}
	bool load(std::string filePath, std::string id, SDL_Renderer* renderer);
	void TextureManager::draw(std::string id, SDL_Rect rectangle, SDL_Renderer* renderer, SDL_RendererFlip flip);
	void TextureManager::drawAnimated(std::string id, int phase, SDL_Rect rectangle, SDL_Renderer* renderer, SDL_RendererFlip flip);
	void TextureManager::clearFromTextureMap(std::string id);

	std::map<std::string, SDL_Texture*> textureMap;
private:
	static TextureManager* instance;
};

