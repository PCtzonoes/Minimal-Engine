#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "GameObjectManager.hpp"
#include "AssetManager.hpp"
#include "Component.hpp"

class TileComponent : public Component {
private:
	SDL_Texture *_texture;
	SDL_Rect _sourceRect;
	SDL_Rect _destinationRect;
	glm::vec2 _position;

public:
	TileComponent(int sourceRectX, int sourceRectY,
	              int destX, int destY,
	              int tileSize, int tileScale,
	              std::string assetTextureId) {
		_texture = Game::GlobalAssetManager->GetTexture(assetTextureId);

		_sourceRect.x = sourceRectX;
		_sourceRect.y = sourceRectY;
		_sourceRect.w = tileSize;
		_sourceRect.h = tileSize;

		_destinationRect.x = destX;
		_destinationRect.y = destY;
		_destinationRect.w = tileSize * tileScale;
		_destinationRect.h = tileSize * tileScale;

		_position.x = destX;
		_position.y = destY;
	}

	~TileComponent() {
		SDL_DestroyTexture(_texture);
	}

	void Update(uint32_t deltaTime) override {
		_destinationRect.x = _position.x - Game::Camera.x;
		_destinationRect.y = _position.y - Game::Camera.y;
	}

	void Render() override {
		TextureManager::Draw(_texture, _sourceRect, _destinationRect, SDL_FLIP_NONE);
	}
};
