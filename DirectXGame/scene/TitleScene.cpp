#include "TitleScene.h"

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();

	viewProjection_.Initialize();

	modelSkydome_.reset(Model::CreateFromOBJ("Sky", true));

	// スカイドームの生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get(), {0, 0, 0});

	modelCloud_.reset(Model::CreateFromOBJ("cube", true));

	// 雲の生成と初期化
	cloud_ = std::make_unique<Cloud>();
	cloud_->Initialize(modelCloud_.get(),false);

	// 背景スプライト
	titleTexHandle_ = TextureManager::Load("title.png");
	titleSprite_ =
	    Sprite::Create(titleTexHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void TitleScene::Update() {

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
			Sleep(1 * 300);
			sceneNo = SELECT;
		}
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		cloud_->SetMoveFlag(true);
		selectSwitchFlag = true;
	} 
	if (selectSwitchFlag) {
		selectSwitchTimer--;
	}
	if (selectSwitchTimer <= 0) {
		selectSwitchTimer = 0;
		cloud_->SetMoveFlag(false);
		sceneNo = SELECT;
	}
	cloud_->Update();
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

