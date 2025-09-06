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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	testSceneSprite_->Draw();
	gameSceneSprite_->Draw();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion 
}
