#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "IScene.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>

enum Weather {
	SUNNY,
	RAIN,
	SNOW,
	FOG,
	MAX,
};

class SelectScene : public IScene {

public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransformSunny_;
	WorldTransform worldTransformRain_;
	WorldTransform worldTransformSnow_;
	WorldTransform worldTransformFog_;

	ViewProjection viewProjection_;

	std::unique_ptr<Model> selectModel_;

	//　矢印を押したときの番号
	int stageCount_ = 0;

	// 左右のキーを押したときのフラグ
	bool leftFlag_ = false;
	bool rightFlag_ = false;
	
	// 回転の処理
	float degree[4];
	float rotf[4];

	// 移動を管理する変数
	int target_ = 0;
	
	// 初期ポジション
	float position_[4] = {90, 180, 270, 360};
	
	// ステージナンバー
	int stageNo_[4] = {SUNNYSTAGE, RAINSTAGE, SNOWSTAGE, FOGSTAGE};
};
