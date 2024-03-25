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

	WorldTransform worldTransform_;
	WorldTransform worldTransform_2;
	ViewProjection viewProjection_;

	std::unique_ptr<Model> selectModel_;

	std::unique_ptr<Model> selectModel_2;

	uint32_t buttonSound_;

	uint32_t titleTexHandle_ = 0;
	Sprite* sunnySprite_ = nullptr;
	Sprite* rainSprite_ = nullptr;

	int stageCount_ = false;

	bool leftFlag_ = false;

	bool moveLeftFlag_ = false;

	bool rightFlag_ = false;

	Vector3 g;

	float rotf;

	/*float moveStart[3];

	float moveEnd[3];*/

	const float timer = 60;

	const float SelectSpace = 420;

	int weatherNo_[4] = {SUNNY, RAIN, SNOW, FOG};

	Vector3 position_[4];

	// Vector3 position_2 = {}

	int stageNo_[3] = {TITLE, TEST, SUNNYSTAGE};
};
