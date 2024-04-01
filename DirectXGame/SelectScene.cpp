#include "SelectScene.h"
#include "Easings.h"
#include "ImGuiManager.h"
#include "MT.h"
#include <DirectXMath.h>
#include <vector>

void SelectScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransformSunny_.Initialize();

	worldTransformRain_.Initialize();

	worldTransformSnow_.Initialize();

	worldTransformFog_.Initialize();

	viewProjection_.Initialize();

	selectModel_.reset(Model::CreateFromOBJ("cube", true));

	degree[SUNNY] = position_[SUNNY];

	degree[RAIN] = position_[RAIN];

	degree[SNOW] = position_[SNOW];

	degree[FOG] = position_[FOG];

	rotf[SUNNY] = DirectX::XMConvertToRadians(degree[SUNNY]);

	rotf[RAIN] = DirectX::XMConvertToRadians(degree[RAIN]);

	rotf[SNOW] = DirectX::XMConvertToRadians(degree[SNOW]);

	rotf[FOG] = DirectX::XMConvertToRadians(degree[FOG]);
}

void SelectScene::Update() {

	if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {

		leftFlag_ = true;

	} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {

		rightFlag_ = true;
	}

	if (input_->TriggerKey(DIK_1)) {
		rotf[SUNNY] -= DirectX::XMConvertToRadians(1);
	}

	if (input_->PushKey(DIK_2)) {
		rotf[SUNNY] -= DirectX::XMConvertToRadians(1);
	}

	if (leftFlag_ == true) {
		if (degree[SUNNY] != degree[FOG]) {
			degree[SUNNY] -= 1;
			rotf[SUNNY] = DirectX::XMConvertToRadians(degree[SUNNY]);
		} else {
			leftFlag_ = false;
		}
	}

	if (rightFlag_ == true) {
		if (degree[SUNNY] != degree[SUNNY + 1]) {
			degree[SUNNY] += 1;
			rotf[SUNNY] = DirectX::XMConvertToRadians(degree[SUNNY]);
		} else {
			rightFlag_ = false;
		}
	}

	/*if (input_->TriggerKey(DIK_SPACE)) {
	    sceneNo = stageNo_[stageCount_];
	}*/

	worldTransformSunny_.translation_.x = -cosf(rotf[SUNNY]) * 20.0f;
	worldTransformSunny_.translation_.z = -sinf(rotf[SUNNY]) * 20.0f;

	worldTransformRain_.translation_.x = -cosf(rotf[RAIN]) * 20.0f;
	worldTransformRain_.translation_.z = -sinf(rotf[RAIN]) * 20.0f;

	worldTransformSnow_.translation_.x = -cosf(rotf[SNOW]) * 20.0f;
	worldTransformSnow_.translation_.z = -sinf(rotf[SNOW]) * 20.0f;

	worldTransformFog_.translation_.x = -cosf(rotf[FOG]) * 20.0f;
	worldTransformFog_.translation_.z = -sinf(rotf[FOG]) * 20.0f;

	ImGui::Begin("stageNum");

	float position[3]{
	    worldTransformSunny_.translation_.x, worldTransformSunny_.translation_.y,
	    worldTransformSunny_.translation_.z};

	/*float position_2[3]{
	    worldTransform_2.translation_.x, worldTransform_2.translation_.y,
	    worldTransform_2.translation_.z};*/

	ImGui::Text("SelectScene");

	ImGui::Text("rotf %f", rotf[SUNNY]);

	ImGui::Text("degreeSunny %f", degree[SUNNY]);

	ImGui::Text("degreeRain %f", degree[RAIN]);

	ImGui::SliderFloat3("3DPosition", position, -40.0f, 360.0f);

	worldTransformSunny_.translation_ = {position[0], position[1], position[2]};

	ImGui::End();

	worldTransformSunny_.UpdateMatrix();
	worldTransformRain_.UpdateMatrix();
	worldTransformSnow_.UpdateMatrix();
	worldTransformFog_.UpdateMatrix();
}

void SelectScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	selectModel_->Draw(worldTransformSunny_, viewProjection_);

	/*selectModel_->Draw(worldTransformRain_, viewProjection_);

	selectModel_->Draw(worldTransformSnow_, viewProjection_);

	selectModel_->Draw(worldTransformFog_, viewProjection_);*/

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
