#pragma once
#include "Sprite.h"
#include "Tile.h"
#include <string>
#include <vector>

class TileMap {
public:
	TileMap();
	~TileMap();

	void Initialize(int tileWidth, int tileHeight, int mapWidth, int mapHeight);

	void LoadFromCSV(const std::string& filename, const std::string& textureFile);
	void Update();
	void Draw();

private:
	int tileWidth_ = 32;
	int tileHeight_ = 32;
	int mapWidth_ = 0;
	int mapHeight_ = 0;

	Sprite* sprite_ = nullptr;

	uint32_t textureHandle_ = 0;
	std::vector<Tile> tiles_;
};
