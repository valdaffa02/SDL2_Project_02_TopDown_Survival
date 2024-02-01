#include "Map.h"
#include "ECS/Components.h"
#include <fstream>

extern Manager manager;

Map::Map(std::string textureID, int mapScale, int tileSize) : texID(textureID), mapScale(mapScale), tileSize(tileSize)
{
	scaledSize = mapScale * tileSize;
}

Map::~Map()
{

}

void Map::loadMap(std::string path, int sizeX, int sizeY)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			addTile(srcX, srcY, x * scaledSize, y * scaledSize);
			//to ignore the comas
			mapFile.ignore();
		}
	}

	//to ignore the empty line
	mapFile.ignore();

	//to give collision to the map
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			if (c == '1')
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize, scaledSize);
				tcol.addGroup(Game::groupCollider);
			}
			//to ignore the comas
			mapFile.ignore();
		}
	}

	mapFile.close();
}


void Map::addTile(int srcX, int srcY, int xPos, int yPos)
{
	//tile added to manager as entity
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, texID);
	tile.addGroup(Game::groupMap);
}