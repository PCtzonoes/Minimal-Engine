#pragma once

#include <SDL2/SDL.h>

#include <utility>
#include "Component.hpp"
#include "TransformComponent.hpp"
#include "TextureManager.hpp"
#include "AssetManager.hpp"
#include "SpriteAnimation.hpp"

class Component;

class TransformComponent;

class TextureManager;

class AssetManager;

class SpriteAnimation;

class SpriteComponent : public Component {
private:
	TransformComponent *_transform;
	SDL_Texture *_texture;
	SDL_Rect _sourceRect;
	SDL_Rect _destinationRect;

	bool _isAnimated;
	bool _isFixed;
	int _frameCount;
	int _animationSpeed;
	std::map<AnimDirection, SpriteAnimation> _animations;
	AnimDirection _curAnimationName;
	unsigned int _animationIndex = 0;

public:
	SDL_RendererFlip SpriteFlip = SDL_FLIP_NONE;

	SpriteComponent() {
		_isAnimated = false;
		_isFixed = false;
		_sourceRect.x = 0;
		_sourceRect.y = 0;
		_sourceRect.w = 32;
		_sourceRect.h = 32;
		SetTexture("../../Assets/Images/Collision-Texture.png");
	}

	SpriteComponent(const std::string& assetTextureId, int sourceX, int sourceY, int w, int h,
	                bool isFixed = false) {
		_isAnimated = false;
		_isFixed = isFixed;
		_sourceRect.x = sourceX;
		_sourceRect.y = sourceY;
		_sourceRect.w = w;
		_sourceRect.h = h;
		SetTexture(assetTextureId);
	}

	SpriteComponent(const std::string& assetTextureId, int w, int h, int frameCount, int animSpeed,
	                bool hasDirections, bool isFixed) {
		_isAnimated = true;
		_isFixed = isFixed;
		_sourceRect.x = 0;
		_sourceRect.y = 0;
		_sourceRect.w = w;
		_sourceRect.h = h;
		_frameCount = frameCount;
		_animationSpeed = animSpeed;

		if (hasDirections) {
			SpriteAnimation downAnim = SpriteAnimation(0, _frameCount, _animationSpeed);
			SpriteAnimation rightAnim = SpriteAnimation(1, _frameCount, _animationSpeed);
			SpriteAnimation leftAnim = SpriteAnimation(2, _frameCount, _animationSpeed);
			SpriteAnimation upAnim = SpriteAnimation(3, _frameCount, _animationSpeed);

			_animations.emplace(AnimDirection::Down, downAnim);
			_animations.emplace(AnimDirection::Right, rightAnim);
			_animations.emplace(AnimDirection::Left, leftAnim);
			_animations.emplace(AnimDirection::Up, upAnim);

			_animationIndex = 0;
			_curAnimationName = AnimDirection::Down;
		} else {
			SpriteAnimation singleAnim = SpriteAnimation(0, _frameCount, _animationSpeed);
			_animations.emplace(AnimDirection::Single, singleAnim);
			_curAnimationName = AnimDirection::Single;
			_animationIndex = 0;
		}

		SetTexture(assetTextureId);
		Play(_curAnimationName);
	}

	void OnAwake() override {
		_transform = Owner->GetTransform();
	}

	void Update(uint32_t deltaTime) override {
		if (_isAnimated) {
			// Changing to the Animation we need to be playing
			_sourceRect.y = _animationIndex * _sourceRect.h;

			// Changing frame from the animation we're playing
			_sourceRect.x = _sourceRect.w *
			                static_cast<int>(static_cast<int>(SDL_GetTicks() * 0.001f * _animationSpeed) %
			                                 _frameCount);
		}

		_destinationRect.x = static_cast<int>(_transform->Position.x) - (_isFixed ? 0 : Game::Camera.x);
		_destinationRect.y = static_cast<int>(_transform->Position.y) - (_isFixed ? 0 : Game::Camera.y);
		_destinationRect.w = static_cast<int>(_transform->Scale.x * _sourceRect.w);
		_destinationRect.h = static_cast<int>(_transform->Scale.y * _sourceRect.h);
	}

	void Render() override {
		TextureManager::Draw(_texture, _sourceRect, _destinationRect, SpriteFlip);
	}

	// Sprite Specific Functions

	[[nodiscard]] int GetWidth() const {
		return _sourceRect.w;
	}

	[[nodiscard]] int GetHeight() const {
		return _sourceRect.h;
	}

	void SetTexture(const std::string& assetTextureId) {
		_texture = Game::GlobalAssetManager->GetTexture(assetTextureId);
	}

	void Play(AnimDirection animationName) {
		_curAnimationName = animationName;
		SpriteAnimation anim = _animations[_curAnimationName];
		_frameCount = anim.FrameCount;
		_animationIndex = anim.Index;
		_animationSpeed = anim.AnimationSpeed;
	}
};
