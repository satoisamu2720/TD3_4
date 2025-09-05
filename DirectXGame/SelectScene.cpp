#include "SelectScene.h"

void SelectScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	testSceneTexHandle_ = TextureManager::Load("Title.png");
	testSceneSprite_ = Sprite::Create(testSceneTexHandle_, {0, 0});

	gameSceneTexHandle_ = TextureManager::Load("Title.png");
	gameSceneSprite_ = Sprite::Create(gameSceneTexHandle_, {0, 0});
}

void SelectScene::Update() {

	if (input_->TriggerKey(DIK_LEFT)) {
		selectCount = 0;
	}

	if (input_->TriggerKey(DIK_RIGHT)) {
		selectCount = 1;
	}

	if (selectCount = 0 && input_->TriggerKey(DIK_SPACE)) {
		sceneNo = TEST;
	} else if (selectCount = 1 && input_->TriggerKey(DIK_SPACE)) {
		sceneNo = GAME;
	}
	
}

void SelectScene::Draw() {}
