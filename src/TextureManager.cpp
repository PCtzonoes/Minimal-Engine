#include "TextureManager.hpp"

SDL_Texture *TextureManager::LoadTexture(const char *fileName) {
	SDL_Surface *surface = IMG_Load(fileName);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::Renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void TextureManager::Draw(SDL_Texture *texture, SDL_Rect source,
                          SDL_Rect destination, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(Game::Renderer, texture, &source, &destination,
	                 0.0f, nullptr, flip);
}
