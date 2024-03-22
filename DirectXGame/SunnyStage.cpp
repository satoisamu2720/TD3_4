#include "SunnyStage.h"
#include "ImGuiManager.h"

void SunnyStage::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
}

void SunnyStage::Update() {

ImGui::Begin("stage");

	ImGui::Text("SunnyStage");

	ImGui::End();

	
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = SELECT;
	}

}

void SunnyStage::Draw() {}
