#include "GameOverScene.h"
#include "ImGuiManager.h"

void GameOverScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	texHandle_ = TextureManager::Load("gameOver.png");
	sprite_ = Sprite::Create(texHandle_, {0,0});

	Audio::GetInstance()->Audio::PauseWave(BGM_);

	// サウンド

	BGM_ = Audio::GetInstance()->LoadWave("Sound/BGM.mp3");
	cloudSound_ = Audio::GetInstance()->LoadWave("Sound/cloud.mp3");       // 雲
	moveSound_ = Audio::GetInstance()->LoadWave("Sound/button06.mp3");     // ADボタン
	decisionSound_ = Audio::GetInstance()->LoadWave("Sound/button01.mp3"); // 決定ボタン
	summerSound_ = Audio::GetInstance()->LoadWave("Sound/summer.mp3");     // 晴BGM
	gameOverSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameOver.mp3");
	gameClearSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameClear.mp3");
	CarSound_ = Audio::GetInstance()->LoadWave("Sound/Car.mp3"); // 車走行

	Audio::GetInstance()->Audio::PlayWave(gameOverSound_, false, 1.0f);
	Audio::GetInstance()->Audio::PauseWave(CarSound_);
}

void GameOverScene::Update() {

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
			Sleep(1 * 300);
			sceneNo = TITLE;
		}
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
	}

#ifdef _DEBUG
	ImGui::Begin("stageNum");

	ImGui::Text("GameOverScene");

	ImGui::End();
#endif

}

void GameOverScene::Draw() {
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
	sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
