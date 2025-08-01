#include "game.h"
#include <iostream>

int main(int argc, char* argv[])
{
	const int width = 800;
	const int height = 600;
	const int FPS = 75;
	const int frameDelay = 1000 / FPS;

	Game game;
	game.init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

	while (game.running()) {
		Uint32 frameStart = SDL_GetTicks();

		game.handleEvents();
		game.update();
		game.render();

		int frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game.clean();

	return 0;
}
