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

	uint32_t buttonSound_;

	uint32_t titleTexHandle_ = 0;

	int stageCount_ = false;

	bool leftFlag_ = false;

	bool moveLeftFlag_ = false;

	bool rightFlag_ = false;

	float degree[4];

	float rotf[4];

	const float timer = 60;

	const float SelectSpace = 420;

	int weatherNo_[4] = {SUNNY, RAIN, SNOW, FOG};

	const float position_[4] = {90, 180, 270, 0};

	int stageNo_[3] = {TITLE, TEST, SUNNYSTAGE};
};
