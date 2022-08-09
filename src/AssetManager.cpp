#include "AssetManager.hpp"

AssetManager::AssetManager(GameObjectManager *manager) : _manager(manager) {}

AssetManager::~AssetManager() = default;

void AssetManager::ClearData() {
	_textures.clear();
	_fonts.clear();
}

void AssetManager::AddTexture(std::string textureId, const char *filePath) {
	_textures.emplace(textureId, TextureManager::LoadTexture(filePath));
}

SDL_Texture *AssetManager::GetTexture(const std::string &textureId) {
	return _textures[textureId];
}

void AssetManager::AddFont(std::string fontId, const char *filePath,
													 int fontSize) {
	_fonts.emplace(fontId, FontManager::LoadFont(filePath, fontSize));
}

TTF_Font *AssetManager::GetFont(const std::string &fontId) {
	return _fonts[fontId];
}
