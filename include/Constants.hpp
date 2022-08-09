#pragma once

#include <SDL2/SDL.h>

const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 900;

const unsigned int FPS = 60;
const uint32_t FRAME_TARGET_TIME = 1000u / FPS;
const uint32_t MAX_FRAME_TIME = 50u;

enum struct AnimDirection : unsigned char {
	Single = 0,
	Down = 1,
	Right = 2,
	Left = 4,
	Up = 8
};

// LAYERS
const unsigned int LAYERS_COUNT = 6;
enum struct LayerType : unsigned int {
	Tile = 0,
	Vegetation = 1,
	Enemy = 2,
	Player = 3,
	Projectile = 4,
	UI = 5
};

// Collision Tags
const unsigned int TAGS_COUNT = 6;
enum struct CollisionTagType : unsigned int {
	None = 0,
	Environment = 1,
	Player = 2,
	Enemy = 3,
	EnemyBullet = 4,
	PlayerBullet = 5,
};

// Colors

const SDL_Color COLOR_WHITE = {255, 255, 255, 255};
const SDL_Color COLOR_BLACK = {0, 0, 0, 255};
const SDL_Color COLOR_RED = {255, 0, 0, 255};
const SDL_Color COLOR_GREEN = {0, 255, 0, 255};
const SDL_Color COLOR_BLUE = {0, 0, 255, 255};