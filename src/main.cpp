#include "Game.h"



int main(int argc, char* argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	Game* game = new Game();

	game->init("Top Down Survival", 800, 640, false);

	while (game->running())
	{
		frameStart = SDL_GetTicks();
		
		game->handleEvents();
		if (!game->paused())
		{
			game->update();
			game->render();
		}

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();
	

	return 0;
}

