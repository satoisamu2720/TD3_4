#pragma once
#include <cstdint>

struct Tile {
	uint32_t textureHandle = 0; // テクスチャID
	int x = 0;                  // マップ座標X
	int y = 0;                  // マップ座標Y
	bool isCollidable = false;  // 当たり判定ありか？
};
