#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Scene.h"
#include "TextureManager.h"

class TitleScene {

public:
	void Initialize();

	void Update();

	void Draw();

	void Reset();

	bool IsSceneEnd() { return isSceneEnd; }

	Scene NextScene() { return Scene::SELECT; }

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;

	//// サウンド
	//uint32_t bgmHandle_;
	//uint32_t playBGM_;
	//bool isBGM_;

	uint32_t buttonSound_;

	// シーンを終わらせるフラグ
	bool isSceneEnd = false;

	uint32_t titleTexHandle_ = 0;
	Sprite* titleSprite_ = nullptr;

	Vector2 position_;

	//bool motionFlag_ = false;

};
