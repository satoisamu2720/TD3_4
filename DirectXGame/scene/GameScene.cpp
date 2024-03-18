#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "VectraCalculation.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	
	texHandle_ = TextureManager::Load("Box/Tex.png");

  #pragma region プレイヤー初期化
	// 自キャラモデル読み込み
	modelPlayerBody_.reset(Model::CreateFromOBJ("player_Body", true));
	modelPlayerFront_.reset(Model::CreateFromOBJ("player_Front", true));
	modelPlayerBack_.reset(Model::CreateFromOBJ("player_Back", true));


	//自キャラモデル配列
	std::vector<Model*> playerModels = {
	    modelPlayerBody_.get(),
		modelPlayerFront_.get(), 
	 modelPlayerBack_.get(),    
	};
	//プレイヤー初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
  #pragma endregion 
	
  #pragma region 障害物

	//箱モデル読み込み
	BoxModel_ = (Model::CreateFromOBJ("Box", true));

	//箱初期化
	box_ = std::make_unique<Box>();
	box_->Initialize(BoxModel_, {-20.0f, 0.0f, 100.0f});


	//加速装置モデル読み込み
	acceleratorModel_ = (Model::CreateFromOBJ("SpeedUP", true));

	//加速装置初期化
	for (int i = 0; i < 2; i++) {
	accelerator_[i] = std::make_unique<Accelerator>();
	}
	accelerator_[0]->Initialize(acceleratorModel_, {20.0f, -8.0f, 200.0f});
	accelerator_[1]->Initialize(acceleratorModel_, {20.0f, -8.0f, 1600.0f});

  #pragma endregion 

  #pragma region ステージ
	//ステージ外モデル読み込み
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	//外モデル初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_);

	//ステージ地面モデル読み込み
	modelGround_ = Model::CreateFromOBJ("ground", true);
	//地面モデル初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_, {0.0f, -10.0f, 0.0f});


  #pragma endregion 

  #pragma region カメラ
	//レールカメラ初期化
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({0.0f, 20.0f, -200.0f}, {0.0f, 0.0f, 0.0f});
	railCamera_->SetTarget(&player_->GetWorldTransform());
	//追従対象をプレイヤーに
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetViewProjection(&railCamera_->GetViewProjection());
	
	//デバックカメラ初期化
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

   #pragma endregion

	
	worldTransform_.Initialize();
	viewProjection_.Initialize();

}

void GameScene::Update() {

#pragma region 更新処理

	if (weather == 0) {
	player_->Update();
	player_->SunnyUpdate();
	}
	if (weather == 1) {
	player_->Update();
	player_->ThunderstormUpdate();
	}

	box_->Update();
	// 加速装置
	for (int i = 0; i < 2; i++) {
	accelerator_[i]->Update();
	}

	skydome_->Update();
	ground_->Update();

	debugCamera_->Update();

#pragma endregion 

#pragma region カメラセット

	// デバックカメラのifdef


#ifdef _DEBUG
	if (input_->TriggerKey(DIK_LSHIFT) && isDebugcameraActive_ == false) {
		isDebugcameraActive_ = true;
	} else if (input_->TriggerKey(DIK_LSHIFT) && isDebugcameraActive_ == true) {
		isDebugcameraActive_ = false;
	}
#endif

	// カメラ処理
	if (isDebugcameraActive_ == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
#pragma endregion

#ifdef _DEBUG
	ImGui::Begin("weather");
	ImGui::InputFloat("weather", &weather, 1.0f);
	//ImGui::Checkbox("", &);
	ImGui::End();

	
#endif

	// 当たり判定

#pragma region プレイヤーの当たり判定

	PlayerBackZ_ = player_->GetWorldPosition().z - 26.0f;
	PlayerFlontZ_ = player_->GetWorldPosition().z + 20.0f;
	PlayerLeftX_ = player_->GetWorldPosition().x - 15.0f;
	PlayerRightX_ = player_->GetWorldPosition().x + 15.0f;

#pragma endregion

#pragma region プレイヤーとボックスの当たり判定


	BoxBackZ_ = box_->GetWorldPosition().z - 5.0f;
	BoxFlontZ_ = box_->GetWorldPosition().z + 5.0f;
	BoxLeftX_ = box_->GetWorldPosition().x - 5.0f;
	BoxRightX_ = box_->GetWorldPosition().x + 5.0f;

	if ((PlayerLeftX_ < BoxRightX_ && PlayerRightX_ > BoxLeftX_) &&
	    (BoxFlontZ_ > PlayerBackZ_ && BoxBackZ_ < PlayerFlontZ_)) {
		if (timerFlag == false) {
		player_->SetNormalHit(true);
			player_->SetThunderHit(true);
			timerFlag = true;
		}
	}

#pragma endregion

#pragma region プレイヤーと加速装置の当たり判定
	// 加速装置
	for (int i = 0; i < 2; i++) {
		SpeedBackZ_ = accelerator_[i]->GetWorldPosition().z - 10.0f;
		SpeedFlontZ_ = accelerator_[i]->GetWorldPosition().z + 10.0f;
		SpeedLeftX_ = accelerator_[i]->GetWorldPosition().x - 30.0f;
		SpeedRightX_ = accelerator_[i]->GetWorldPosition().x + 30.0f;

		if ((PlayerLeftX_ < SpeedRightX_ && PlayerRightX_ > SpeedLeftX_) &&
		    (SpeedFlontZ_ > PlayerBackZ_ && SpeedBackZ_ < PlayerFlontZ_)) {

			railCamera_->SetIsSpeedUp(true);
			// player_->SetPosition({0.0f, 0.0f, -50.0f});
		}
	}

#pragma endregion

	Time();
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	player_->Draw(viewProjection_);

	//skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	box_->Draw(viewProjection_);
	// 加速装置
	for (int i = 0; i < 2; i++) {
		accelerator_[i]->Draw(viewProjection_);
	}

	Model::PostDraw();

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる

	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
}

void GameScene::Time() {
	if (timerFlag == true) {
		timer++;
	}
	if (timer >= 120) {
		timer = 0;
		timerFlag = false;
	}
}

