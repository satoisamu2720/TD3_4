#include "GameOverScene.h"
#include "ImGuiManager.h"

void GameOverScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	texHandle_ = TextureManager::Load("gameClear.png");
	buttonhandle_ = TextureManager::Load("PlayerHP3.png");

	sprite_ = Sprite::Create(texHandle_, {0, 0});
	buttonSprite_ = Sprite::Create(buttonhandle_, {0, 0});
}

void GameOverScene::Update() {

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = SELECT;
	}
}

void GameOverScene::Draw() {
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

	buttonSprite_->Draw();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}