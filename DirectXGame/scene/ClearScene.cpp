#include "ClearScene.h"
#include "ImGuiManager.h"

void ClearScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	texHandle_ = TextureManager::Load("gameClear.png");
	sprite_ = Sprite::Create(texHandle_, {0, 0});
}

void ClearScene::Update() {

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = SELECT;
	}
	if (input_->IsTriggerMouse(0)) {
		sceneNo = SELECT;
	}
#ifdef _DEBUG
	ImGui::Begin("stageNum");

	ImGui::Text("ClearScene");

	ImGui::End();
#endif
}

void ClearScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	sprite_->Draw();

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
	sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
