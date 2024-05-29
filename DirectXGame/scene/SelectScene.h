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
#include "stage/skydome/Skydome.h"
#include "stage/cloud/Cloud.h"
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

	WorldTransform worldTransformSky_;

	ViewProjection viewProjection_;

	uint32_t titleTexHandle_ = 0;
	Sprite* Sprite_ = nullptr;

	// 晴
	std::unique_ptr<Model> sunModel_;
	// 雨
	std::unique_ptr<Model> rainModel_;
	// 雪
	std::unique_ptr<Model> snowModel_;
	std::unique_ptr<Model> snowHatModel_;
	// 霧
	std::unique_ptr<Model> fogModel_;

	// Skydomeの3Dモデル
	std::unique_ptr<Model> modelSkydome_;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;

	// 雲の3Dモデル
	std::unique_ptr<Model> modelCloud_;

	// 雲
	std::unique_ptr<Cloud> cloud_;

	// 　矢印を押したときの番号
	int stageCount_ = 2;

	// 左右のキーを押したときのフラグ
	bool leftFlag_ = false;
	bool rightFlag_ = false;

	bool selectSwitchFlag = true;
	float selectSwitchTimer = 120;

	// 回転の処理
	float degree[4];
	float rotf[4];

	// 移動を管理する変数
	int target_ = 0;

	// 初期ポジション
	float position_[4] = {90, 180, 270, 360};

	// ステージナンバー
	int stageNo_[4] = {SUNNYSTAGE, RAINSTAGE, SNOWSTAGE, FOGSTAGE};

	
	// 読み込みサウンド
	uint32_t cloudSound_;
	uint32_t decisionSound_;
	uint32_t moveSound_;

	//サウンドフラグ
	bool setFlag_ = false;
	
};
