#pragma once
#include <cstdint>
#include <vector>
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"

enum class MapchipType {
	NONE, //空白
	BLOCK, //ブロック
};


struct MapChipData {
	std::vector<std::vector<MapchipType>> data;
};



class MapChipFiled {

	public:


	MapChipFiled();
	 ~MapChipFiled();

	void Initilize();
	void Update();
	void Draw();
	void ReseMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapchipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	
	Vector3 GetMapChipPostionByIndex(uint32_t xIndex, uint32_t yIndex);

	// 追加：縦横のブロック数を返すゲッター
	uint32_t GetNumBlockVertical() const { return kNumBlockVertical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	//ブロックの個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;
	MapChipData mapChipData_;

	private:
	
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection View_;
	

};
