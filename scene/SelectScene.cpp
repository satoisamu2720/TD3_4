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

	modelSkydome_.reset(Model::CreateFromOBJ("Sky", true));

	// スカイドームの生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get(), {0, 0, 0});

	modelCloud_.reset(Model::CreateFromOBJ("cloud", true));

	// 雲の生成と初期化
	cloud_ = std::make_unique<Cloud>();
	cloud_->Initialize(modelCloud_.get(), true);
	selectSwitchTimer = 120;

	// 背景スプライト
	// titleTexHandle_ = TextureManager::Load("sky.png");
	// Sprite_ =
	//    Sprite::Create(titleTexHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

#pragma region モデル

	sunModel_.reset(Model::CreateFromOBJ("sunMark", true));

	rainModel_.reset(Model::CreateFromOBJ("rain", true));

	snowModel_.reset(Model::CreateFromOBJ("Snowman", true));

	fogModel_.reset(Model::CreateFromOBJ("fog", true));

#pragma endregion

#pragma region 初期ポジション

	degree[SUNNY] = position_[SUNNY];

	degree[RAIN] = position_[RAIN];

	degree[SNOW] = position_[SNOW];

	degree[FOG] = position_[FOG];

#pragma endregion

	rotf[SUNNY] = DirectX::XMConvertToRadians(degree[SUNNY]);

	rotf[RAIN] = DirectX::XMConvertToRadians(degree[RAIN]);

	rotf[SNOW] = DirectX::XMConvertToRadians(degree[SNOW]);

	rotf[FOG] = DirectX::XMConvertToRadians(degree[FOG]);

	worldTransformSunny_.scale_ = {2, 2, 2};

	worldTransformRain_.scale_ = {2, 2, 2};

	worldTransformSnow_.scale_ = {2, 2, 2};

	worldTransformFog_.scale_ = {1, 1, 1};

	// サウンド

	BGM_ = Audio::GetInstance()->LoadWave("Sound/BGM.mp3");
	cloudSound_ = Audio::GetInstance()->LoadWave("Sound/cloud.mp3");       // 雲
	moveSound_ = Audio::GetInstance()->LoadWave("Sound/button06.mp3");     // ADボタン
	decisionSound_ = Audio::GetInstance()->LoadWave("Sound/button01.mp3"); // 決定ボタン
	summerSound_ = Audio::GetInstance()->LoadWave("Sound/summer.mp3");     // 晴BGM
	gameOverSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameOver.mp3");
	gameClearSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameClear.mp3");

	// Audio::GetInstance()->Audio::PauseWave(BGM_);
}

void SelectScene::Update() {

	Audio::GetInstance()->Audio::ResumeWave(BGM_);

	if (stageCount_ == 0 && summerON) {
		// Audio::GetInstance()->Audio::PlayWave(summerSound_, true, 1.0f);
		summerON = false;
	}

	if (selectSwitchFlag) {
		cloud_->SetMoveFlag(true);
		selectSwitchTimer--;
	}
	if (selectSwitchTimer <= 0) {
		cloud_->SetMoveFlag(false);
	}

	if (selectSwitchTimer == 119) {

		Audio::GetInstance()->Audio::PlayWave(cloudSound_, false, 1.0f);
	}
	cloud_->Update();

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.sThumbLX < -100 && leftFlag_ == false && rightFlag_ == false) {
			stageCount_--;
			leftFlag_ = true;
			Audio::GetInstance()->Audio::PlayWave(moveSound_, false, 1.0f);
		}

		if (joyState.Gamepad.sThumbLX > 100 && leftFlag_ == false && rightFlag_ == false) {
			stageCount_++;
			rightFlag_ = true;
			Audio::GetInstance()->Audio::PlayWave(moveSound_, false, 1.0f);
		}
	}
	if (input_->TriggerKey(DIK_LEFT) && leftFlag_ == false && rightFlag_ == false ||
	    input_->TriggerKey(DIK_A) && leftFlag_ == false && rightFlag_ == false) {
		stageCount_--;
		leftFlag_ = true;
		setFlag_ = false;
		Audio::GetInstance()->Audio::PlayWave(moveSound_, false, 1.0f);
	}

	if (input_->TriggerKey(DIK_RIGHT) && leftFlag_ == false && rightFlag_ == false ||
	    input_->TriggerKey(DIK_D) && leftFlag_ == false && rightFlag_ == false) {
		stageCount_++;
		rightFlag_ = true;
		setFlag_ = false;
		Audio::GetInstance()->Audio::PlayWave(moveSound_, false, 1.0f);
	}

	if (stageCount_ != 0) {
		// Audio::GetInstance()->Audio::PauseWave(summerSound_);
	} else {
		// Audio::GetInstance()->Audio::ResumeWave(summerSound_);
	}

	if (stageCount_ > 3) {
		stageCount_ = 0;
	} else if (stageCount_ < 0) {
		stageCount_ = 3;
	}

	if (leftFlag_ == true && rightFlag_ == false) {
		target_++;

		if (target_ >= 90) {
			leftFlag_ = false;
			target_ = 0;
		}

		for (int i = 0; i < 4; i++) {
			if (target_ != 90) {
				degree[i] += 1;
				rotf[i] = DirectX::XMConvertToRadians(degree[i]);
			}
		}
	}

	if (rightFlag_ == true && leftFlag_ == false) {
		target_++;

		if (target_ >= 90) {
			rightFlag_ = false;
			target_ = 0;
		}

		for (int i = 0; i < 4; i++) {
			if (target_ != 90) {
				degree[i] -= 1;
				rotf[i] = DirectX::XMConvertToRadians(degree[i]);
			}
		}
	}

	if (input_->TriggerKey(DIK_SPACE) && leftFlag_ == false && rightFlag_ == false) {
		sceneNo = stageNo_[stageCount_];

		Audio::GetInstance()->Audio::PlayWave(decisionSound_, false, 1.0f);
	}
	if (input_->TriggerKey(DIK_H)) {

		Audio::GetInstance()->Audio::PauseWave(summerSound_);
	}
	if (input_->TriggerKey(DIK_SPACE) || joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A 
		&&leftFlag_ == false && rightFlag_ == false) {
		Sleep(1 * 300);
		sceneNo = stageNo_[stageCount_];
		stageCount_ = 0;
	}

	worldTransformSunny_.translation_.x = -cosf(rotf[SUNNY]) * 20.0f;
	worldTransformSunny_.translation_.z = -sinf(rotf[SUNNY]) * 20.0f;

	worldTransformRain_.translation_.x = -cosf(rotf[RAIN]) * 20.0f;
	worldTransformRain_.translation_.z = -sinf(rotf[RAIN]) * 20.0f;

	worldTransformSnow_.translation_.x = -cosf(rotf[SNOW]) * 20.0f;
	worldTransformSnow_.translation_.z = -sinf(rotf[SNOW]) * 20.0f;

	worldTransformFog_.translation_.x = -cosf(rotf[FOG]) * 20.0f;
	worldTransformFog_.translation_.z = -sinf(rotf[FOG]) * 20.0f;

#ifdef _DEBUG

	ImGui::Begin("stageNum");

	float position[3]{
	    worldTransformSunny_.translation_.x, worldTransformSunny_.translation_.y,
	    worldTransformSunny_.translation_.z};

	/*float position_2[3]{
	    worldTransform_2.translation_.x, worldTransform_2.translation_.y,
	    worldTransform_2.translation_.z};*/

	ImGui::Text("StageCount %d", stageCount_);

	ImGui::Text("rotf %d", leftFlag_);

	ImGui::Text("Speed%d", target_);

	ImGui::Text("%d", summerSound_);

	ImGui::Text("degreeSunny %f", degree[SUNNY]);

	ImGui::Text("degreeRain %f", degree[RAIN]);

	ImGui::Text("degreeSnow %f", degree[SNOW]);

	ImGui::Text("degreeFog %f", degree[FOG]);

	ImGui::SliderFloat3("3DPosition", position, -40.0f, 360.0f);

	ImGui::Text("selectSwitchTimer %f", selectSwitchTimer);
	ImGui::Checkbox("selectSwitchFlag", &selectSwitchFlag);

	ImGui::End();

#endif
	// worldTransformSunny_.translation_ = {position[0], position[1], position[2]};
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

	// Sprite_->Draw();

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

	sunModel_->Draw(worldTransformSunny_, viewProjection_);

	rainModel_->Draw(worldTransformRain_, viewProjection_);

	snowModel_->Draw(worldTransformSnow_, viewProjection_);

	fogModel_->Draw(worldTransformFog_, viewProjection_);

	cloud_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);

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
