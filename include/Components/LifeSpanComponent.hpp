#pragma once

#include "Component.hpp"

class LifeSpanComponent : public Component {
private:
	uint32_t _lifeSpan = 1.0f;
	uint32_t _timer;

public:
	LifeSpanComponent(uint32_t lifeSpan = 1.0f) { _lifeSpan = lifeSpan; }

	void OnEnable() override { _timer = 0u; }

	void Update(uint32_t deltaTime) override {
		if (_timer >= _lifeSpan) {
			Owner->Destroy();
		}
		_timer += deltaTime;
	}
};
