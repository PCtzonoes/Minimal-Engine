#pragma once

#include "GameObjectManager.hpp"
#include "RigidbodyComponent.hpp"
#include "TransformComponent.hpp"
#include <glm/glm.hpp>

class ProjectileEmitterComponent : public Component {
private:
	TransformComponent *_transform;
	TransformComponent *_shooterTransform;
	RigidbodyComponent *_rb;

	glm::vec2 _origin;
	glm::vec2 _offset;
	float     _speed;
	float     _range;
	float     _angleRad;
	bool      _isLooping;

public:
	ProjectileEmitterComponent(TransformComponent *shooterTransform, int offsetX,
														 int offsetY, float speed, int angleDeg,
														 float range, bool isLooping = false) {
		_shooterTransform = shooterTransform;
		_offset.x         = offsetX;
		_offset.y         = offsetY;
		_angleRad         = glm::radians(static_cast<float>(angleDeg));
		_speed            = speed;
		_range            = range;
		_isLooping        = isLooping;
	}

	void OnAwake() override {
		_transform = Owner->GetTransform();
		_origin    = glm::vec2(_shooterTransform->Position.x + _offset.x,
													 _shooterTransform->Position.y + _offset.y);

		_rb = Owner->GetComponent<RigidbodyComponent>();
		if (_rb == nullptr) {
			_rb = &Owner->AddComponent<RigidbodyComponent>(0, 0, 0.0f);
		}
		_rb->Velocity =
				glm::vec2(glm::cos(_angleRad) * _speed, glm::sin(_angleRad) * _speed);
	}

	void Update(uint32_t deltaTime) override {
		if (glm::distance(_transform->Position, _origin) >= _range) {
			if (_isLooping) {
				if (_shooterTransform == nullptr) // FIXME: Figure this one out
				{
					std::cout << "No More Bullet" << std::endl;
					Owner->Destroy();
					return;
				}
				_origin = glm::vec2(_shooterTransform->Position.x + _offset.x,
														_shooterTransform->Position.y + _offset.y);
				_transform->Position.x = _origin.x;
				_transform->Position.y = _origin.y;
			} else {
				Owner->Destroy();
			}
		}
	}
};
