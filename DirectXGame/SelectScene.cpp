#include "SelectScene.h"
#include "Easings.h"
#include "ImGuiManager.h"
#include "MT.h"
#include <vector>

void SelectScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();

	worldTransform_2.Initialize();

	viewProjection_.Initialize();

	selectModel_.reset(Model::CreateFromOBJ("cube", true));

	selectModel_2.reset(Model::CreateFromOBJ("cube", true));

	worldTransform_.translation_ = {0, 0, 0};
	worldTransform_2.translation_ = {-20.0, 0, 40};

	//// 背景スプライト
	// titleTexHandle_ = TextureManager::Load("Select.png");

	// sunnySprite_ =
	//     Sprite::Create(titleTexHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	// rainSprite_ =
	//     Sprite::Create(titleTexHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void SelectScene::Update() {

	// Vector2 position_ = {sunnySprite_->GetPosition()};

	if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
		if (stageCount_ >= 1) {
			stageCount_ -= 1;
			leftFlag_ = true;

			/*g = GetBezierCurve(
			    worldTransform_.translation_, worldTransform_2.translation_,
			    TrianglePositionXZ(worldTransform_.translation_, worldTransform_2.translation_),
			    timer);*/

			// worldTransform_.translation_ = Add(g, worldTransform_.translation_);

			worldTransform_.translation_.x += sinf(3.14f) * 1.0f;
			worldTransform_.translation_.z -= cosf(3.14f) * 1.0f;
		}
	} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
		if (stageCount_ < 2) {
			stageCount_ += 1;
			rightFlag_ = true;
		}
	}

	if (input_->PushKey(DIK_1)) {
		/*worldTransform_.translation_.x -= worldTransform_.translation_.x * cosf(3.14f) * sinf(3.14f);
		worldTransform_.translation_.z -= sinf(3.14f) * cosf(3.14f);*/

		 rotf -= 0.05f;

		worldTransform_.translation_.x = -cosf(rotf) * worldTransform_.translation_.x;
		 worldTransform_.translation_.z = -sinf(rotf) * worldTransform_.translation_.z;


	}

	/*if (input_->TriggerKey(DIK_SPACE)) {
	    sceneNo = stageNo_[stageCount_];
	}*/

	ImGui::Begin("stageNum");

	float position[3]{
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	float position_2[3]{
	    worldTransform_2.translation_.x, worldTransform_2.translation_.y,
	    worldTransform_2.translation_.z};

	ImGui::Text("SelectScene");

	ImGui::Text("Count%d", stageCount_);

	ImGui::SliderFloat3("3DPosition", position, -40.0f, 40.0f);

	ImGui::SliderFloat3("3DPosition2", position_2, -40.0f, 40.0f);

	worldTransform_.translation_ = {position[0], position[1], position[2]};

	worldTransform_2.translation_ = {position_2[0], position_2[1], position_2[2]};

	ImGui::End();

	worldTransform_.UpdateMatrix();
	worldTransform_2.UpdateMatrix();
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

	selectModel_->Draw(worldTransform_, viewProjection_);

	selectModel_2->Draw(worldTransform_2, viewProjection_);

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
