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
	// 自キャラモデル
	modelPlayerBody_.reset(Model::CreateFromOBJ("player_Body", true));
	modelPlayerFront_.reset(Model::CreateFromOBJ("player_Front", true));
	modelPlayerL_.reset(Model::CreateFromOBJ("player_Left", true));
	modelPlayerR_.reset(Model::CreateFromOBJ("player_Right", true));
	modelPlayerBack_.reset(Model::CreateFromOBJ("player_Back", true));
	BoxModel_.reset(Model::CreateFromOBJ("Box", true));

	//自キャラモデル配列
	std::vector<Model*> playerModels = {
	    modelPlayerBody_.get(),
		modelPlayerFront_.get(), 
		modelPlayerL_.get(),
	    modelPlayerR_.get(),    
		modelPlayerBack_.get(),
	};
	//プレイヤー初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
  #pragma endregion 

	/*modelSkydome_ = Model::CreateFromOBJ("sky", true);
	modelGround_ = Model::CreateFromOBJ("ground", true);*/
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	boxTransform_.Initialize();

	/*skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_);

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_, {1.0f, -2.0f, 0.0f});*/

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({0.0f, 0.0f, -30.0f}, {0.0f, 0.0f, 0.0f});

	player_->SetViewProjection(&railCamera_->GetViewProjection());

	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	boxTransform_.translation_ = {10.0f, +2.0f, 0.0f};
}

void GameScene::Update() {
	player_->Update();
	/*skydome_->Update();
	ground_->Update();*/

	debugCamera_->Update();
	// デバックカメラのifdef

	boxTransform_.UpdateMatrix();

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

	

	//当たり判定

	for (Player* player : players_) {

		PlayerBackZ_ = player->GetWorldTransformBack().z - 2.0f;
		PlayerFlontZ_ = player->GetWorldTransformfront().z + 2.0f;
		PlayerLeftX_ = player->GetWorldTransformL().x + 4.5f;
		PlayerRightX_ = player->GetWorldTransformR().x - 4.5f;

		BoxBackZ_ = boxTransform_.translation_.z - 0.5f;
		BoxFlontZ_ = boxTransform_.translation_.z + 0.5f;
		BoxRightX_ = boxTransform_.translation_.x + 0.5f;
		BoxLeftX_ = boxTransform_.translation_.x - 0.5f;

		if ((PlayerLeftX_ < BoxRightX_ && PlayerRightX_ > BoxLeftX_) &&
		    (BoxFlontZ_ > PlayerBackZ_ && BoxBackZ_ < PlayerFlontZ_)) 
		{
			Vector3 tmpTranslate = player->GetWorldPosition();
			tmpTranslate.z += 10.0f;
			player_->SetTranslate(tmpTranslate);
		}
	}

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
	BoxModel_->Draw(boxTransform_, viewProjection_);
	/*skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);*/
	Model::PostDraw();

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる

	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
}

