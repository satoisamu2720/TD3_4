#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

// マップチップの種類
enum class MapchipType {
	NONE,  // 空白
	BLOCK, // ブロック
};

// CSVなどで保持するマップチップデータ
struct MapChipData {
	std::vector<std::vector<MapchipType>> data;
};

class MapChipField {
public:
	MapChipField();
	~MapChipField();

	// 初期化・更新・描画
	void Initialize(Model* blockModel, ViewProjection* view);
	void Update();
	void Draw();

	// CSV読み込み
	void LoadMapChipCsv(const std::string& filePath);

	// 座標取得
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) const;
	MapchipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) const;

	// ゲッター
	uint32_t GetNumBlockVertical() const { return kNumBlockVertical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

private:
	void ResetMapChipData();
	void GenerateBlocks();

private:
	// マップデータ
	MapChipData mapChipData_;

	// ワールド変換とモデル
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	Model* model_ = nullptr;
	ViewProjection* view_ = nullptr;

	// マップ設定
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	// CSV → MapchipType の変換テーブル
	std::map<std::string, MapchipType> mapChipTable_ = {
	    {"0", MapchipType::NONE },
        {"1", MapchipType::BLOCK}
    };
};
