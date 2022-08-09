#pragma once

#include <glm/glm.hpp>
#include "TransformComponent.hpp"

class RigidbodyComponent : public Component {
public:
	glm::vec2 Velocity{};
	float GravityScale = 1.0f;

	RigidbodyComponent() {
		Velocity = glm::vec2(0, 0);
	}

	RigidbodyComponent(int velX, int velY, float gravityScale) {
		Velocity = glm::vec2(velX, velY);
		GravityScale = gravityScale;
	}

	RigidbodyComponent(glm::vec2 vel, float gravityScale) {
		Velocity = vel;
		GravityScale = gravityScale;
	}

	void Update(uint32_t deltaTime) override {
		auto time = static_cast<float>(deltaTime);
		Velocity += Game::Gravity * GravityScale * time;

		Owner->GetTransform()->Translate(Velocity * time);
	}
};
