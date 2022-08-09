#pragma once

#include <iostream>

#include "ColliderComponent.hpp"
#include "Component.hpp"
#include "Constants.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "GameObjectManager.hpp"
#include "RigidbodyComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"

class KeyboardInputComponent : public Component {
private:
	TransformComponent *_transform;
	RigidbodyComponent *_rb;
	SpriteComponent *_sprite;

	SDL_Keycode _upKey;
	SDL_Keycode _downKey;
	SDL_Keycode _leftKey;
	SDL_Keycode _rightKey;
	SDL_Keycode _shootKey;

	int _moveSpeed = 100;
	int _bulletSpeed = 167;
	float _velX = 0;
	float _velY = 0;

public:
	KeyboardInputComponent() {}

	KeyboardInputComponent(SDL_Keycode upKey, SDL_Keycode downKey,
	                       SDL_Keycode leftKey, SDL_Keycode rightKey,
	                       SDL_Keycode shootKey) {
		_upKey = upKey;
		_downKey = downKey;
		_leftKey = leftKey;
		_rightKey = rightKey;
		_shootKey = shootKey;
	}

	KeyboardInputComponent(std::string upKey, std::string downKey,
	                       std::string leftKey, std::string rightKey,
	                       std::string shootKey) {
		_upKey = GetSDLKeyCode(upKey);
		_downKey = GetSDLKeyCode(downKey);
		_leftKey = GetSDLKeyCode(leftKey);
		_rightKey = GetSDLKeyCode(rightKey);
		_shootKey = GetSDLKeyCode(shootKey);
	}

	SDL_Keycode GetSDLKeyCode(std::string k) {
		if (k.length() == 0) {
			return SDLK_UNKNOWN;
		}
		if (k.length() == 1) {
			return static_cast<SDL_Keycode>(std::tolower(k[0]));
		}

		std::for_each(k.begin(), k.end(), [](char &c) { c = ::tolower(c); });

		if (k == "space") {
			return SDLK_SPACE;
		}
		if (k == "up") {
			return SDLK_UP;
		}
		if (k == "down") {
			return SDLK_DOWN;
		}
		if (k == "left") {
			return SDLK_LEFT;
		}
		if (k == "right") {
			return SDLK_RIGHT;
		}

		return SDLK_UNKNOWN;
	}

	void OnAwake() override {
		_transform = Owner->GetTransform();
		_rb = Owner->GetComponent<RigidbodyComponent>();
		if (_rb == nullptr) {
			_rb = &Owner->AddComponent<RigidbodyComponent>(0, 0, 0.0f);
		}
		_sprite = Owner->GetComponent<SpriteComponent>();
	}

	void Update(uint32_t deltaTime) override {
		if (_rb == nullptr) {
			return;
		}
		if (_sprite == nullptr) {
			return;
		}

		if (Game::Event.type == SDL_KEYDOWN) {
			SDL_Keycode kc = Game::Event.key.keysym.sym;

			if (kc == _upKey) {
				_sprite->Play(AnimDirection::Up);
				_velY = -_bulletSpeed;
				_velX = 0;
				_rb->Velocity.y = -_moveSpeed;
				_rb->Velocity.x = 0;
			}
			if (kc == _downKey) {
				_sprite->Play(AnimDirection::Down);
				_velY = _bulletSpeed;
				_velX = 0;
				_rb->Velocity.y = _moveSpeed;
				_rb->Velocity.x = 0;
			}
			if (kc == _leftKey) {
				_sprite->Play(AnimDirection::Left);
				_velX = -_bulletSpeed;
				_velY = 0;
				_rb->Velocity.x = -_moveSpeed;
				_rb->Velocity.y = 0;
			}
			if (kc == _rightKey) {
				_sprite->Play(AnimDirection::Right);
				_velX = _bulletSpeed;
				_velY = 0;
				_rb->Velocity.x = _moveSpeed;
				_rb->Velocity.y = 0;
			}
			if (kc == _shootKey) {
				ShootBullet();
			}
		}
		if (Game::Event.type == SDL_KEYUP) {
			SDL_Keycode kc = Game::Event.key.keysym.sym;

			if (kc == _upKey || kc == _downKey) {
				_rb->Velocity.y = 0;
			}
			if (kc == _leftKey || kc == _rightKey) {
				_rb->Velocity.x = 0;
			}
			if (kc == _shootKey) {
			}
		}
	}

	void ShootBullet() {
		GameObject &bullet = Owner->GetManager()->AddGameObject(
				"Bullet", _transform->Position.x + _sprite->GetWidth() * 0.5f,
				_transform->Position.y + _sprite->GetHeight() * 0.5f, 2, 2,
				LayerType::Projectile);
		bullet.AddComponent<RigidbodyComponent>(_velX, _velY, 0.0f);
		bullet.AddComponent<SpriteComponent>("player-bullet", 0, 0, 4, 4);
		bullet.AddComponent<LifeSpanComponent>(5.0f);
		bullet.AddComponent<ColliderComponent>(CollisionTagType::PlayerBullet, 8, 8,
		                                       0, 0, nullptr);
	}
};
