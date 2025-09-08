//#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "IScene.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
// #include "stage/cloud/Cloud.h"
// #include "stage/skydome/Skydome.h"

class TitleScene : public IScene {

public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;

	WorldTransform worldTransformSky_;

	WorldTransform worldTransformTitle_;

	WorldTransform worldTransformCredit_[2];

	WorldTransform worldTransformA_;

	ViewProjection viewProjection_;

	// Skydomeの3Dモデル
	// std::unique_ptr<Model> modelSkydome_;

	// スカイドーム
	// std::unique_ptr<Skydome> skydome_;

	// 雲の3Dモデル
	// std::unique_ptr<Model> modelCloud_;

	// 雲
	// std::unique_ptr<Cloud> cloud_;

	// std::unique_ptr<Model> modelTitle_;

	// std::unique_ptr<Model> modelCredit_[2];

	// std::unique_ptr<Model> modelAButton_;

	//// サウンド
	// uint32_t bgmHandle_;
	// uint32_t playBGM_;
	// bool isBGM_;

	uint32_t titleTexHandle_ = 0;
	Sprite* titleSprite_ = nullptr;

	Vector2 position_;

	bool selectSwitchFlag = false;
	float selectSwitchTimer = 120;

	// サウンド再生フラグ

	Vector4 color = {0, 0, 0, 0.9f};

	// 読み込みサウンド
	/*uint32_t BGM_;
	uint32_t cloudSound_;
	uint32_t gameOverSound_;
	uint32_t gameClearSound_;
	uint32_t decisionSound_;
	uint32_t moveSound_;
	uint32_t summerSound_;
	bool on = true;*/
};
