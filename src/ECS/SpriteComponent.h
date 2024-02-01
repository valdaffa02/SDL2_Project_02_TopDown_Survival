#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:

	int animIndex = 0;
	std::map<std::string, Animation*> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated, std::string animId, int animIndex, int animFrames, int animSpeed)
	{
		animated = isAnimated;

		addAnimation(animId, animIndex, animFrames, animSpeed);
		/*Animation idle = Animation(0, 3, 100);
		Animation walk = Animation(1, 8, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);*/

		play(animId);

		setTex(id);
	}

	~SpriteComponent()
	{
	}

	void setTex(std::string id)
	{
		texture = Game::assets->getTexture(id);
		//std::cout << "texture: " << id << std::endl;
	}

	void init() override
	{

		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
		
	}

	void update() override
	{

		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x - Game::camera.x);
		destRect.y = static_cast<int>(transform->position.y - Game::camera.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;

		//std::cout << destRect.x <<" " << destRect.y << std::endl;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void play(std::string animName)
	{
		if (animations.find(animName) != animations.end())
		{
			frames = animations[animName]->frames;
			animIndex = animations[animName]->index;
			speed = animations[animName]->speed;
		}
		
	}

	void addAnimation(std::string id, int index, int frames, int speed)
	{
		Animation* newAnim = new Animation(index, frames, speed);
		animations.emplace(id, newAnim);
	}

};