#pragma once

#include "ECS.h"
#include "../Vector2D.h"
#include "../Game.h"
#include "../TextureManager.h"
#include "../AssetManager.h"



class TileComponent : public Component
{
private:

public:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;


	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int destX, int destY, int tileSize, int tileScale, std::string id)
	{
		texture = Game::assets->getTexture(id);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tileSize;
		position.x = static_cast<float>(destX);
		position.y = static_cast<float>(destY);
		destRect.w = destRect.h = tileSize * tileScale;
	}

	void update() override
	{
		destRect.x = static_cast<int>(position.x - Game::camera.x);
		destRect.y = static_cast<int>(position.y - Game::camera.y);
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};