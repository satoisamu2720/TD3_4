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
class SelectScene : public IScene {

public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t backSceneTexHandle_ = 0;
	Sprite* backSceneSprite_ = nullptr;

	uint32_t testSceneTexHandle_ = 0;
	Sprite* testSceneSprite_ = nullptr;

	uint32_t gameSceneTexHandle_ = 0;
	Sprite* gameSceneSprite_ = nullptr;

	uint32_t tutorialTexHandle_ = 0;
	Sprite* tutorialSprite_ = nullptr;

	uint32_t stageTexHandle_ = 0;
	Sprite* stageSprite_ = nullptr;

	int selectCount = 0;

};
