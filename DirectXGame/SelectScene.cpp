#include "SelectScene.h"

void SelectScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
}

void SelectScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = SELECT;
	} 

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = GAME;
	} 

}

void SelectScene::Draw() {}
