#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "AssetManager.hpp"
#include "FontManager.hpp"
#include "Game.hpp"
#include "GameObjectManager.hpp"
#include <utility>

class TextLabelComponent : public Component {
private:
	TransformComponent *_transform;
	SDL_Texture        *_texture;

	SDL_Rect    _position;
	SDL_Color   _color;
	std::string _text;
	std::string _fontId;

public:
	TextLabelComponent(std::string text, std::string fontId,
										 const SDL_Color &color) {
		_text   = std::move(text);
		_fontId = std::move(fontId);
		_color  = color;
		SetLabelText(_text);
	}

	void SetLabelText(std::string text) {
		_text                = std::move(text);
		SDL_Surface *surface = TTF_RenderText_Blended(
				Game::GlobalAssetManager->GetFont(_fontId), _text.c_str(), _color);
		_texture = SDL_CreateTextureFromSurface(Game::Renderer, surface);
		SDL_FreeSurface(surface);
	}

	void SetLabelFont(std::string fontId) {
		_fontId = std::move(fontId);
		SetLabelText(_text);
	}

	void SetLabelColor(SDL_Color color) {
		_color = color;
		SetLabelText(_text);
	}

	void OnAwake() override {
		_transform  = Owner->GetTransform();
		_position.x = _transform->Position.x;
		_position.y = _transform->Position.y;
		_position.w = _transform->Scale.x;
		_position.h = _transform->Scale.y;
		SDL_QueryTexture(_texture, nullptr, nullptr, &_position.w, &_position.h);
	}

	void Render() override { FontManager::Draw(_texture, _position); }
};
