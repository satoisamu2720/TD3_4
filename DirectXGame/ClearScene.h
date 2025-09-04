#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "IScene.h"

class ClearScene : public IScene {

public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;

	//// サウンド
	// uint32_t bgmHandle_;
	// uint32_t playBGM_;
	// bool isBGM_;

	//uint32_t buttonSound_;

	uint32_t texHandle_ = 0;
	Sprite* sprite_ = nullptr;

	// 読み込みサウンド
	/*uint32_t BGM_;
	uint32_t cloudSound_;
	uint32_t gameOverSound_;
	uint32_t gameClearSound_;
	uint32_t decisionSound_;
	uint32_t moveSound_;
	uint32_t summerSound_;*/

};
