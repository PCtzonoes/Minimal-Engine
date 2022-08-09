#include "Constants.hpp"
#include "Game.hpp"
#include <iostream>

int main() {
	std::cout << "Welcome to my awesome GameEngine" << std::endl;

	Game *game = new Game();
	game->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, false);

	while (Game::IsRunning()) {
		game->Update();
	}

	game->Destroy();
}
