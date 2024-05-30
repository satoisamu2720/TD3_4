#include "TitleScene.h"

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// サウンド

	BGM_ = Audio::GetInstance()->LoadWave("Sound/BGM.mp3");
	cloudSound_ = Audio::GetInstance()->LoadWave("Sound/cloud.mp3");       // 雲
	moveSound_ = Audio::GetInstance()->LoadWave("Sound/button06.mp3");     // ADボタン
	decisionSound_ = Audio::GetInstance()->LoadWave("Sound/button01.mp3"); // 決定ボタン
	summerSound_ = Audio::GetInstance()->LoadWave("Sound/summer.mp3");     // 晴BGM
	gameOverSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameOver.mp3");
	gameClearSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameClear.mp3");


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

	modelSkydome_.reset(Model::CreateFromOBJ("Sky", true));

	// スカイドームの生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get(), {0, 0, 0});

	modelCloud_.reset(Model::CreateFromOBJ("cloud", true));

	modelTitle_.reset(Model::CreateFromOBJ("title", true));
	
	modelCredit_[0].reset(Model::CreateFromOBJ("credit", true));
	modelCredit_[1].reset(Model::CreateFromOBJ("credit2", true));

	modelAButton_.reset(Model::CreateFromOBJ("AButton", true));

	// 雲の生成と初期化
	cloud_ = std::make_unique<Cloud>();
	cloud_->Initialize(modelCloud_.get(),false);

	// 背景スプライト
	titleTexHandle_ = TextureManager::Load("title.png");
	titleSprite_ =
	    Sprite::Create(titleTexHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	
}

void TitleScene::Update() {
	if (on) {
	Audio::GetInstance()->Audio::PlayWave(BGM_, true, 1.0f);
		on = false;
	} else {
		
	}

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
			Sleep(1 * 300);
			cloud_->SetMoveFlag(true);
			selectSwitchFlag = true;
			Audio::GetInstance()->Audio::PlayWave(cloudSound_, false, 1.0f);
			sceneNo = SELECT;
			
		}
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		cloud_->SetMoveFlag(true);
		selectSwitchFlag = true;
		Audio::GetInstance()->Audio::PlayWave(cloudSound_, false, 1.0f);
		
	} 
	if (selectSwitchFlag) {
		selectSwitchTimer--;
	}
	
	if (selectSwitchTimer <= 0) {
		selectSwitchTimer = 60;
		selectSwitchFlag = false;
		cloud_->SetMoveFlag(false);
		sceneNo = SELECT;
	}
	cloud_->Update();

	worldTransformCredit_[0].UpdateMatrix();
	worldTransformCredit_[1].UpdateMatrix();
	worldTransformTitle_.UpdateMatrix();
	worldTransformA_.UpdateMatrix();
}

void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	//titleSprite_->Draw();

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
	cloud_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	modelTitle_->Draw(worldTransformTitle_, viewProjection_);
	modelCredit_[0]->Draw(worldTransformCredit_[0], viewProjection_);
	modelCredit_[1]->Draw(worldTransformCredit_[1], viewProjection_);
	modelAButton_->Draw(worldTransformA_, viewProjection_);

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



