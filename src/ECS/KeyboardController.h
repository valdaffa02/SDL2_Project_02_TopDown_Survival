#pragma once

#include "../Game.h"
#include "Components.h"
#include <unordered_map>
#include <functional>

// Custom hash function for SDL_KeyCode

class KeyboardController : public Component
{
	private:
		TransformComponent* transform;
		SpriteComponent* sprite;
		//std::map<int, bool> keyControlMap;

		bool key[4] = { false, false, false, false };

	public:
		void init() override
		{
			transform = &entity->getComponent<TransformComponent>();
			sprite = &entity->getComponent<SpriteComponent>();
		}

		void update() override
		{

			const Uint8* state = SDL_GetKeyboardState(NULL);


			if (state[SDL_SCANCODE_A])
			{
				key[0] = true;
			}
			else {
				key[0] = false;
			}

			if (state[SDL_SCANCODE_S])
			{
				key[1] = true;
			}
			else {
				key[1] = false;
			}
			if (state[SDL_SCANCODE_D])
			{
				key[2] = true;
			}
			else {
				key[2] = false;
			}
			if (state[SDL_SCANCODE_W])
			{
				key[3] = true;
			}
			else {
				key[3] = false;
			}

			

			// Update velocity based on key states
			transform->velocity.x = key[2] - key[0];
			transform->velocity.y = key[1] - key[3];




			//Handle Animation
			if (key[0])
			{
				sprite->play("walk_hor");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			}
			else if (!key[0] && transform->face == 2 && (transform->velocity.x == 0 && transform->velocity.y == 0))
			{
				sprite->play("idle2");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			}

			if (key[1])
			{
				sprite->play("walk_vert_front");
			}
			else if (!key[1] && transform->face == 0 && (transform->velocity.x == 0 && transform->velocity.y == 0))
			{
				sprite->play("idle1");
			}

			if (key[2])
			{
				sprite->play("walk_hor");
				sprite->spriteFlip = SDL_FLIP_NONE;
			}
			else if (!key[2] && transform->face == 3 && (transform->velocity.x == 0 && transform->velocity.y == 0))
			{
				sprite->play("idle2");
				sprite->spriteFlip = SDL_FLIP_NONE;
			}

			if (key[3])
			{
				sprite->play("walk_vert_back");
			}
			else if (!key[3] && transform->face == 1 && (transform->velocity.x == 0 && transform->velocity.y == 0))
			{
				sprite->play("idle3");
			}
		}
};