#pragma once
#include <string>



class Map
{
private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;

public:
	Map(std::string textureID, int mapScale, int tileSize);

	~Map();

	void loadMap(std::string path, int sizeX, int sizeY);
	void addTile(int srcX, int srcY, int xPos, int yPos);
};