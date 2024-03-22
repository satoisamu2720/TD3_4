#include "SelectScene.h"
#include "ImGuiManager.h"
#include "MT.h"
#include "Easings.h"
#include <vector>

void SelectScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();

	selectModel_.reset(Model::CreateFromOBJ("cube" ,true));

	selectModel2_.reset(Model::CreateFromOBJ("cube", true));

	// 背景スプライト
	titleTexHandle_ = TextureManager::Load("Select.png");

	sunnySprite_ =
	    Sprite::Create(titleTexHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	rainSprite_ =
	    Sprite::Create(titleTexHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	// std::vector<Sprite*> selectSprite = {sunnySprite_};
}

void SelectScene::Update() {

	//Vector2 position_ = {sunnySprite_->GetPosition()};

	if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
		if (stageCount_ >= 1) {
			stageCount_ -= 1;
			leftFlag_ = true;
		}
	} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
		if (stageCount_ < 2) {
			stageCount_ += 1;
			rightFlag_ = true;
		}
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = stageNo_[stageCount_];
	}

	if (leftFlag_) {
		GetBezierCurve(
		    static_cast<Vector3>(position_[].x), static_cast<Vector3>(position_[1].x] + SelectSpace),
		    TrianglePositionXZ(position_.x,position_.x+SelectSpace,position_.z,position_.z));
	}

	if (rightFlag_) {
	}

	//sunnySprite_->SetPosition(position_);

	ImGui::Begin("stageNum");

	float position[2]{position_.x, position_.y};

	ImGui::Text("SelectScene");

	ImGui::Text("Count%d", stageCount_);

	ImGui::SliderFloat2("TexturePosition", position, 10.0f, 1280.0f);

	position_ = {position[0], position[1]};

	//sunnySprite_->SetPosition(position_);

	ImGui::End();
}

void SelectScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	sunnySprite_->Draw();
	// SelectSprite_->Draw();

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
