#include "TileMap.h"
#include "TextureManager.h"
#include <fstream>
#include <sstream>

TileMap::TileMap() {}
TileMap::~TileMap() {}

void TileMap::Initialize(int tileWidth, int tileHeight, int mapWidth, int mapHeight) {
	tileWidth_ = tileWidth;
	tileHeight_ = tileHeight;
	mapWidth_ = mapWidth;
	mapHeight_ = mapHeight;
	tiles_.resize(mapWidth * mapHeight);
	sprite_ = sprite_

}

void TileMap::LoadFromCSV(const std::string& filename, const std::string& textureFile) {
	textureHandle_ = TextureManager::Load(textureFile);

	std::ifstream file(filename);
	std::string line;
	int y = 0;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;
		int x = 0;
		while (std::getline(ss, cell, ',')) {
			int id = std::stoi(cell);
			Tile& tile = tiles_[y * mapWidth_ + x];
			tile.textureHandle = textureHandle_;
			tile.x = x;
			tile.y = y;
			tile.isCollidable = (id != 0); // 0‚È‚ç‹ó”’A1ˆÈã‚È‚ç•Ç
			x++;
		}
		y++;
	}
}

void TileMap::Update() {
	// ¡‚Í“Á‚Éˆ—‚È‚µ
}

void TileMap::Draw() {
	for (const auto& tile : tiles_) {
		if (!tile.isCollidable)
			continue;

		sprite_->Draw(textureHandle_, {(float)(tile.x * tileWidth_), (float)(tile.y * tileHeight_)});
	}
}
