#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <vector>

class AssetManager;

class Game
{
	private:
		SDL_Window *window;
		

	public:

		Game();
		~Game();

		static int mouse_x, mouse_y;

		void init(const char* title, int width, int height, bool fullscreen);

		void handleEvents();
		void update();
		void render();
		void clean();

		void spawnEnemy(SDL_Rect& spawnRect);
		//Vector2D getVelocity(Vector2D source, float x, float y);

		static SDL_Renderer* renderer;
		static SDL_Event event;
		static AssetManager* assets;
		static SDL_Rect camera;
		static bool isRunning;
		static bool isPaused;

		bool running() { return isRunning; }
		bool paused() { return isPaused; }

		enum groupLabels : std::size_t
		{
			groupPlayer,
			groupEnemy,
			groupMap,
			groupCollider,
			groupText,
			groupProjectile,
			groupUI
		};


};