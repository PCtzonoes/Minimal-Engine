#pragma once

#include <SDL2/SDL.h>
#include <vector>

class ColliderComponent;

class Collision {
public:
	static bool CheckRectangleCollision(const SDL_Rect &rectA,
																			const SDL_Rect &rectB);

	static std::vector<ColliderComponent *> Colliders;
};