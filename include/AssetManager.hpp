#pragma once

#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "GameObjectManager.hpp"

class AssetManager {
private:
	GameObjectManager *_manager;
	std::map<std::string, SDL_Texture *> _textures;
	std::map<std::string, TTF_Font *> _fonts;

public:
	explicit AssetManager(GameObjectManager *manager);

	~AssetManager();

	void ClearData();

	void AddTexture(std::string textureId, const char *filePath);

	SDL_Texture *GetTexture(const std::string& textureId);

	void AddFont(std::string fontId, const char *filePath, int fontSize);

	TTF_Font *GetFont(const std::string& fontId);
};
