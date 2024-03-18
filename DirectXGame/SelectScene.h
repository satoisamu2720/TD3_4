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
	Sprite* sunnySprite_ = nullptr;
	Sprite* rainSprite_ = nullptr;

	int stageCount_ = 0;

	bool moveLeftFlag_ = 0;

	bool moveRightFlag_ = 0;

	float moveStart[3];

	float moveEnd[3];

	const float SelectSpace = 420;

	int stageNo_[3] = {TITLE, TEST,SUNNYSTAGE};
};
