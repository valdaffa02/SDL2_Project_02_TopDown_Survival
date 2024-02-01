#pragma once

#include <string>
#include <SDL.h>
#include "Components.h"
#include "../TextureManager.h"


class ColliderComponent : public Component
{
private:

public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	TransformComponent* transform;

	ColliderComponent(std::string tag) : tag(tag)
	{}

	ColliderComponent(std::string tag, int xPos, int yPos, int width, int height) : tag(tag)
	{
		collider.x = xPos;
		collider.y = yPos;
		collider.w = width;
		collider.h = height;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		texture = TextureManager::LoadTexture("assets/ColTex.png");
		srcRect = { 0, 0, 32, 32 };
		destRect = { collider.x, collider.y, collider.w, collider.h };
	}

	void update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}

		destRect.x = collider.x - Game::camera.x;
		destRect.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};