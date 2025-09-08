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
#include "TitleScene.h"

class GameOverScene : public IScene {

public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;
	// uint32_t buttonSound_;

	uint32_t texHandle_ = 0;
	uint32_t buttonhandle_ = 0;

	Sprite* sprite_ = nullptr;
	Sprite* buttonSprite_ = nullptr;
};
