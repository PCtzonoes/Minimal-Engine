#include "Constants.hpp"
#include "Game.hpp"
#include <iostream>

int main() {
	std::cout << "Welcome to my awesome GameEngine" << '\n';

	std::cout << "Initializing Game..." << '\n';
	Game *game = new Game();
	game->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	std::cout << "Game initialized" << '\n';

	std::cout << "Starting Game Loop..." << '\n';
	while (Game::IsRunning()) {
		game->Update();
	}

	std::cout << "Game Loop ended" << '\n';
	game->Destroy();
	std::cout << "Game destroyed" << std::endl;

	return 0;
}
