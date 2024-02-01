#include "Game.h"
#include "Map.h"
#include "Collision.h"
#include "ECS/Components.h"
#include "AssetManager.h"

Map* map;
Manager manager;
SDL_Renderer* Game::renderer = nullptr;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

//std::unordered_map<int, bool> globalKeyStates;

int score;

SDL_Event Game::event;

bool Game::isRunning;
bool Game::isPaused;

int Game::mouse_x;
int Game::mouse_y;

Uint32 timePoint1;
Uint32 timePoint2;

AssetManager* Game::assets = new AssetManager(&manager);

Game::Game(){}

Game::~Game(){}

auto& textMouse(manager.addEntity());
auto& textPlayer(manager.addEntity());
auto& player(manager.addEntity());


void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flag = 0;

	if (fullscreen)
	{
		flag = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flag);

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created" << std::endl;
		}

		isRunning = true;
		isPaused = false;
		score = 0;

		timePoint1 = SDL_GetTicks();
		timePoint2 = SDL_GetTicks();
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error: TTF failed to Initalize!" << std::endl;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
	{
		std::cout << "Error: Mixer OpenAudio failed!" << std::endl;
	}


	assets->addFont("arial_16", "assets/arial.ttf", 16);
	assets->addFont("arial_32", "assets/arial.ttf", 32);

	assets->addTexture("player", "assets/test_player_anim.png");
	assets->addTexture("terrain", "assets/serene_terrain_ss.png");
	assets->addTexture("projectile", "assets/proj.png");
	assets->addTexture("enemy", "assets/red_ball.png");

	assets->addTexture("health", "assets/health.png");
	assets->addTexture("healthFrame", "assets/health_frame.png");

	map = new Map("terrain", 2, 32);

	//load map tile layout file
	map->loadMap("assets/map.map", 25, 20);
	
	SDL_Color black = { 0, 0, 0, 255 };

	textMouse.addComponent<UILabel>(10, 10, "test", "arial_16", black);
	textPlayer.addComponent<UILabel>(10, 32, "test", "arial_16", black);

	player.addComponent<StatComponent>(120, 2, 20);
	player.addComponent<TransformComponent>(600.0f, 600.0f, 32, 16, 2, player.getComponent<StatComponent>().speed);
	player.addComponent<SpriteComponent>("player", true, "idle1", 0, 6, 100);
	player.getComponent<SpriteComponent>().addAnimation("idle2", 1, 6, 100);
	player.getComponent<SpriteComponent>().addAnimation("idle3", 2, 6, 100);
	player.getComponent<SpriteComponent>().addAnimation("walk_vert_front", 3, 6, 100);
	player.getComponent<SpriteComponent>().addAnimation("walk_hor", 4, 6, 100);
	player.getComponent<SpriteComponent>().addAnimation("walk_vert_back", 5, 6, 100);
	player.addComponent<ColliderComponent>("player");


	player.addComponent<KeyboardController>();

	textMouse.addGroup(Game::groupText);
	textPlayer.addGroup(Game::groupText);


	player.addGroup(Game::groupPlayer);
}


auto& texts(manager.getGroup(Game::groupText));
auto& players(manager.getGroup(Game::groupPlayer));
auto& tiles(manager.getGroup(Game::groupMap));
auto& colliders(manager.getGroup(Game::groupCollider));
auto& projectiles(manager.getGroup(Game::groupProjectile));
auto& enemies(manager.getGroup(Game::groupEnemy));
auto& uiElements(manager.getGroup(Game::groupUI));



void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&Game::mouse_x, &Game::mouse_y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
				case SDL_BUTTON_LEFT:
					assets->createProjectile(player.getComponent<TransformComponent>().position, Vector2D(static_cast<float>(Game::mouse_x + Game::camera.x), static_cast<float>(Game::mouse_y + Game::camera.y)), 400, 5, "projectile");
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}



void Game::update()
{

	// Handle Timing
	/*
	timePoint2 = SDL_GetTicks();
	float fElapsedTime = static_cast<float>(timePoint2 - timePoint1) / 1000.0f;
	timePoint1 = timePoint2;*/

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	//dynamicRect playerDynaCol = { player.getComponent<ColliderComponent>().collider, player.getComponent<TransformComponent>().velocity * 100.0f * fElapsedTime };
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	textMouse.getComponent<UILabel>().SetLabelText("Health: " + std::to_string(player.getComponent<StatComponent>().health[0]), "arial_16");
	textPlayer.getComponent<UILabel>().SetLabelText("Score: " + std::to_string(score), "arial_16");
	//textEnemy.getComponent<UILabel>().SetLabelText("Target Position: " + std::to_string(enemy1.getComponent<TrackerComponent>().targetPosition.x) + ", " + std::to_string(enemy1.getComponent<TrackerComponent>().targetPosition.y) + " Enemy Velocity: " + std::to_string(enemy1.getComponent<TransformComponent>().velocity.x) + ", " + std::to_string(enemy1.getComponent<TransformComponent>().velocity.y), "arial_16");

	if (enemies.size() < 5)
	{
		SDL_Rect spawnRect;

		spawnRect.x = 128 + camera.x;
		spawnRect.y = 128 + camera.y;
		spawnRect.h = 192 + camera.h;
		spawnRect.w = 192 + camera.w;
	
		spawnEnemy(spawnRect);
	}

	//Vector2D contactPoint, contactNormal;
	//float contactTime;
	
	//WORK IN PROGRESS
	for (auto& collider : colliders)
	{
		/*WIP
		dynamicRect col = {collider->getComponent<ColliderComponent>().collider, collider->getComponent<TransformComponent>().velocity};

		if (Collision::dynamicRectVsRect(playerDynaCol, col, contactPoint, contactNormal, contactTime, fElapsedTime))
		{
			player.getComponent<TransformComponent>().velocity.Zero();
			std::cout << player.getComponent<TransformComponent>().velocity << player.getComponent<TransformComponent>().position <<  std::endl;
		}*/

		SDL_Rect col = collider->getComponent<ColliderComponent>().collider;

		if (Collision::AABB(col, playerCol))
		{
			int xOverlap = abs(std::min(playerCol.x + playerCol.w, col.x + col.w) - std::max(playerCol.x, col.x));
			int yOverlap = abs(std::min(playerCol.y + playerCol.h, col.y + col.h) - std::max(playerCol.y, col.y));

			

			// Choose the smaller overlap to determine the direction of the collision
			if (xOverlap > 0 || yOverlap > 0)
			{
				std::cout << (xOverlap * 100) / playerCol.w << ", " << (yOverlap * 100) / playerCol.h << std::endl;
				if (((xOverlap * 100) / playerCol.w) > ((yOverlap * 100) / playerCol.h))
				{
					if (playerCol.y < col.y)
						playerCol.y -= yOverlap;
					else
						playerCol.y += yOverlap;

					player.getComponent<TransformComponent>().position.y = playerPos.y;
				}
				else
				{
					if (playerCol.x < col.x)
						playerCol.x -= xOverlap;
					else
						playerCol.x += xOverlap;

					player.getComponent<TransformComponent>().position.x = playerPos.x;
				}
			}
		}
	}


	//for projectile & enemy collision
	for (auto& projectile : projectiles)
	{
		for (auto& enemy : enemies)
		{
			if (Collision::AABB(projectile->getComponent<ColliderComponent>().collider, enemy->getComponent<ColliderComponent>().collider))
			{
				
				projectile->destroy();
				enemy->getComponent<StatComponent>().health[0] -= player.getComponent<StatComponent>().attackDmg;
				//std::cout << enemy->getComponent<StatComponent>().health[0] << " " << enemy->getComponent<StatComponent>().isAlive << std::endl;

				if (enemy->getComponent<StatComponent>().health[0] <= 0)
				{
					//std::cout << "Enemy Destroyed" << std::endl;
					enemy->destroy();
					score++;
				}
			}

			
		}

		for (auto& collider : colliders)
		{
			if (Collision::AABB(projectile->getComponent<ColliderComponent>().collider, collider->getComponent<ColliderComponent>().collider))
			{
				std::cout << "Wall!" << std::endl;
				projectile->destroy();
			}
		}
	}

	for (auto& enemy : enemies)
	{
		if (Collision::AABB(playerCol, enemy->getComponent<ColliderComponent>().collider))
		{
			Uint32 currentColTime = SDL_GetTicks();
			if (currentColTime - player.getComponent<TransformComponent>().lastCollisionTime > 500)
			{
				std::cout << "playerHit!" << currentColTime  << " - " << player.getComponent<TransformComponent>().lastCollisionTime << std::endl;
				player.getComponent<TransformComponent>().position += Vector2D(enemy->getComponent<TransformComponent>().velocity.x * 32, enemy->getComponent<TransformComponent>().velocity.y * 32);
				player.getComponent<StatComponent>().health[0] -= enemy->getComponent<StatComponent>().attackDmg;
				player.getComponent<TransformComponent>().lastCollisionTime = currentColTime;
			}
			
		}
	}

	if (player.getComponent<StatComponent>().health[0] <= 0)
	{
		isRunning = false;
	}

	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 400);
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 320);


	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > camera.w)
	{
		camera.x = camera.w;
	}
	if (camera.y > camera.h)
	{
		camera.y = camera.h;
	}
}


void Game::render()
{
	SDL_RenderClear(renderer);

	for (auto& tile : tiles)
	{
		tile->draw();
	}

	for (auto& collider : colliders)
	{
		collider->draw();
	}

	for (auto& player : players)
	{
		player->draw();
	}

	for (auto& enemy : enemies)
	{
		enemy->draw();
	}

	for (auto& projectile : projectiles)
	{
		projectile->draw();
	}
	
	for (auto& text : texts)
	{
		text->draw();
	}

	for (auto& uiElement : uiElements)
	{
		uiElement->draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

/*Vector2D Game::getVelocity(Vector2D source, float x, float y)
{
	
}*/


void Game::spawnEnemy(SDL_Rect& spawnRect)
{
	// Generate random coordinates within spawnRect
	Vector2D spawnPoint;
	spawnPoint.x = spawnRect.x + static_cast<double>(rand()) / RAND_MAX * ((spawnRect.x + spawnRect.w) - spawnRect.x);
	spawnPoint.y = spawnRect.y + static_cast<double>(rand()) / RAND_MAX * ((spawnRect.y + spawnRect.h) - spawnRect.y);

	// Adjust the coordinates if the point is inside cameraRect
	if (spawnPoint.x >= camera.x && spawnPoint.x <= (camera.x + camera.w) &&
		spawnPoint.y >= camera.y && spawnPoint.y <= (camera.y + camera.h)) {
		// Shift the point to the right or left, and up or down, to ensure it's outside B
		spawnPoint.x += (rand() % 2 == 0 ? 1 : -1) * ((camera.x + camera.w) - camera.x);
		spawnPoint.y += (rand() % 2 == 0 ? 1 : -1) * ((camera.y + camera.y) - camera.y);
	}

	auto& enemy(manager.addEntity());
	if(score < 10)
	{
		enemy.addComponent<StatComponent>(60, 1, 20);
	}
	else
	{
		enemy.addComponent<StatComponent>(60, score / 10, 20);
	}
	
	enemy.addComponent<TransformComponent>(spawnPoint.x, spawnPoint.y, 32, 32, 2, enemy.getComponent<StatComponent>().speed);
	enemy.addComponent<SpriteComponent>("enemy");
	enemy.addComponent<ColliderComponent>("enemy");
	enemy.addComponent<TrackerComponent>(&player);

	enemy.addGroup(Game::groupEnemy);
}
