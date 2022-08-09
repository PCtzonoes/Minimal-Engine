#pragma once

#include "Component.hpp"
#include "Game.hpp"
#include "GameObjectManager.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class TransformComponent : public Component {

public:
	glm::vec2 Position{0.0f, 0.0f};
	glm::vec2 Scale{1.0f, 1.0f};

	TransformComponent() {
		Position = glm::vec2(0, 0);
		Scale    = glm::vec2(1, 1);
	}

	TransformComponent(int posX, int posY, int scaleX, int scaleY) {
		Position = glm::vec2(posX, posY);
		Scale    = glm::vec2(scaleX, scaleY);
	}

	void Translate(float deltaX, float deltaY) {
		Position.x += deltaX;
		Position.y += deltaY;
	}

	void Translate(glm::vec2 delta) { Position += delta; }
};