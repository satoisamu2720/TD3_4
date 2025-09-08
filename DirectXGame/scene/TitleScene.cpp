#include "TitleScene.h"

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// サウンド

	// BGM_ = Audio::GetInstance()->LoadWave("Sound/BGM.mp3");
	// cloudSound_ = Audio::GetInstance()->LoadWave("Sound/cloud.mp3");       // 雲
	// moveSound_ = Audio::GetInstance()->LoadWave("Sound/button06.mp3");     // ADボタン
	// decisionSound_ = Audio::GetInstance()->LoadWave("Sound/button01.mp3"); // 決定ボタン
	// summerSound_ = Audio::GetInstance()->LoadWave("Sound/summer.mp3");     // 晴BGM
	// gameOverSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameOver.mp3");
	// gameClearSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameClear.mp3");

	titleTexHandle_ = TextureManager::Load("Title.png");
	titleSprite_ = Sprite::Create(titleTexHandle_, {0, 0});

	worldTransform_.Initialize();

	viewProjection_.Initialize();

	worldTransformTitle_.Initialize();

	worldTransformA_.Initialize();

	worldTransformA_.translation_ = {0, -10, 0};

	worldTransformA_.scale_ = {4, 4, 4};

	worldTransformCredit_[1].Initialize();
	worldTransformCredit_[0].Initialize();

	worldTransformCredit_[0].translation_ = {0, -18, 0};
	worldTransformCredit_[1].translation_ = {15, -18, 0};

	worldTransformTitle_.scale_ = {2, 2, 2};

	// 背景スプライト
	titleTexHandle_ = TextureManager::Load("title.png");
	titleSprite_ =
	    Sprite::Create(titleTexHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void TitleScene::Update() {

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = SELECT;
	}
}

void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// titleSprite_->SetColor(color);
	// Sprite::PreDraw(dxCommon_->GetCommandList(), Sprite::BlendMode::kNormal);
	titleSprite_->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// titleSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>

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