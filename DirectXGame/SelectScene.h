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

class SelectScene : public IScene {

public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	std::unique_ptr<Model> selectModel_;

	uint32_t buttonSound_;

	uint32_t titleTexHandle_ = 0;
	Sprite* titleSprite_ = nullptr;
	Sprite* SelectSprite_ = nullptr;

	int stageCount_ = 0;

	float stageMove_ = 0;

	int stageNo_[3] = {TITLE, TEST,SUNNYSTAGE};
};
