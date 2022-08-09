#pragma once

#include "Game.hpp"

class TextureManager {
public:
	static SDL_Texture *LoadTexture(const char *fileName);

	static void Draw(SDL_Texture *texture, SDL_Rect source,
	                 SDL_Rect destination, SDL_RendererFlip flip);
};
