#pragma once

#include <algorithm>
#include "Game.hpp"
#include "Collision.hpp"
#include "GameObjectManager.hpp"
#include "TransformComponent.hpp"

class ColliderComponent : public Component {
private:
	TransformComponent *_transform;
	int _offsetX;
	int _offsetY;
	std::function<void(ColliderComponent *)> _onCollision;

public:
	CollisionTagType ColliderTag;
	SDL_Rect Collider;
	SDL_Rect DestinationRect;

	ColliderComponent(CollisionTagType colTag, int w, int h, int offsetX = 0, int offsetY = 0,
	                  std::function<void(ColliderComponent *)> onCollision = nullptr) {
		ColliderTag = colTag;
		Collider = {0, 0, w, h};
		_offsetX = offsetX;
		_offsetY = offsetY;
		_onCollision = onCollision;
	}

	void OnAwake() override {
		Collision::Colliders.emplace_back(this);
		_transform = Owner->GetTransform();
		DestinationRect = {Collider.x, Collider.y, Collider.w, Collider.h};
	}

	void Update(uint32_t deltaTime) override {
		Collider.x = static_cast<int>(_transform->Position.x - _offsetX);
		Collider.y = static_cast<int>(_transform->Position.y - _offsetY);

		DestinationRect.x = Collider.x - Game::Camera.x;
		DestinationRect.y = Collider.y - Game::Camera.y;
	}

	void Render() override {
		if (Game::DebugMode) {
			SDL_SetRenderDrawColor(Game::Renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(Game::Renderer, &DestinationRect);
		}
	}

	void OnDestroy() override {
		auto itr = std::remove(Collision::Colliders.begin(), Collision::Colliders.end(), this);
		Collision::Colliders.erase(itr, Collision::Colliders.end());
	}

	void OnCollisionWith(ColliderComponent *other) {
		if (_onCollision != nullptr) {
			_onCollision(other);
		}
	}
};
