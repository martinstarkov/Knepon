#pragma once

class TextureManager {
public:
	static TextureManager* getInstance() {
		if (instance == 0) {
			instance = new TextureManager();
		}
		return instance;
	}
	static bool load(std::string filePath);
	static void TextureManager::draw(std::string texturePath, SDL_Rect rectangle, SDL_RendererFlip flip);
	static void TextureManager::clearFromTextureMap(std::string id);
private:
	static TextureManager* instance;
	static std::map<std::string, SDL_Texture*> textureMap;
};

