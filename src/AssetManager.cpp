#include "AssetManager.h"
#include "ECS\Components.h"


AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::createProjectile(Vector2D sourcePos, Vector2D targetPos, int range, int speed, std::string id)
{
	auto& projectile(manager->addEntity());

	Vector2D direction = (targetPos - sourcePos);
	direction.Normalize();
	projectile.addComponent<TransformComponent>(sourcePos.x, sourcePos.y, 32, 32, 1, speed);
	projectile.addComponent<SpriteComponent>(id);
	projectile.addComponent<ProjectileComponent>(range, speed, direction);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectile);
}

void AssetManager::addTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::getTexture(std::string id)
{
	return textures[id];
}

void AssetManager::addFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}
TTF_Font* AssetManager::getFont(std::string id)
{
	return fonts[id];
}


void AssetManager::addFxWav(std::string id, const char* path)
{
	fxs.emplace(id, Mix_LoadWAV(path));
}

void AssetManager::playFxWav(std::string id)
{
	Mix_PlayChannel(-1, fxs[id], 0);
}