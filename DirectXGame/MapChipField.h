#pragma once
#include <cstdint>
#include <vector>

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipType {
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	//ƒuƒƒbƒN‚ÌŒÂ”
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;


	

};
