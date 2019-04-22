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
	void TextureManager::draw(std::string id, Rectangle rectangle, SDL_Renderer* renderer, SDL_RendererFlip flip);
	void TextureManager::animate(std::string id, double phase, Rectangle rectangle, SDL_Renderer* renderer, SDL_RendererFlip flip);
	std::map<std::string, SDL_Texture*> textureMap;
	void TextureManager::clearFromTextureMap(std::string id);
private:
	static TextureManager* instance;
};

