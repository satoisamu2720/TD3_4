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

class TitleScene : public IScene {

public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Reset();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;

	//// サウンド
	// uint32_t bgmHandle_;
	// uint32_t playBGM_;
	// bool isBGM_;

	uint32_t buttonSound_;

	uint32_t titleTexHandle_ = 0;
	Sprite* titleSprite_ = nullptr;

	Vector2 position_;
};
