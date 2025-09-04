#include "MapChipField.h"

MapChipField::MapChipField() {}
MapChipField::~MapChipField() {
	for (auto& row : worldTransformBlocks_) {
		for (auto* wt : row)
			delete wt;
	}
}

// 初期化
void MapChipField::Initialize(Model* blockModel, ViewProjection* view) {
	model_ = blockModel;
	view_ = view;

	ResetMapChipData();
	GenerateBlocks();
}

void MapChipField::Update() {
	// 現状は静的マップなので何もしない
}

void MapChipField::Draw() {
	if (!model_ || !view_)
		return;

	for (auto& row : worldTransformBlocks_) {
		for (auto* wt : row) {
			if (wt)
				model_->Draw(*wt, *view_);
		}
	}
}

// CSV読み込み
void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	ResetMapChipData();

	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	std::stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();

	for (uint32_t y = 0; y < kNumBlockVertical; ++y) {
		std::string line;
		getline(mapChipCsv, line);
		std::istringstream lineStream(line);

		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			std::string word;
			getline(lineStream, word, ',');

			if (mapChipTable_.contains(word)) {
				mapChipData_.data[y][x] = mapChipTable_[word];
			}
		}
	}

	// CSV読込後にブロック生成
	GenerateBlocks();
}

// ワールド座標取得
Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t x, uint32_t y) const {
	return Vector3(kBlockWidth * x, kBlockHeight * (kNumBlockVertical - 1 - y), 0);
}

// マップチップ取得
MapchipType MapChipField::GetMapChipTypeByIndex(uint32_t x, uint32_t y) const {
	if (x >= kNumBlockHorizontal || y >= kNumBlockVertical)
		return MapchipType::NONE;
	return mapChipData_.data[y][x];
}

// データリセット
void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVertical);
	for (std::vector<MapchipType>& mapChipDataLine : mapChipData_.data)
	{
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

// ワールド変換生成
void MapChipField::GenerateBlocks() {
	worldTransformBlocks_.resize(kNumBlockVertical);
	for (uint32_t y = 0; y < kNumBlockVertical; ++y) {
		worldTransformBlocks_[y].resize(kNumBlockHorizontal, nullptr);
		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			if (mapChipData_.data[y][x] == MapchipType::BLOCK) {
				WorldTransform* wt = new WorldTransform();
				wt->Initialize();
				wt->translation_ = GetMapChipPositionByIndex(x, y);
				worldTransformBlocks_[y][x] = wt;
			}
		}
	}
}
