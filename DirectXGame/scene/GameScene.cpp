#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "player/player.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();
	#pragma region プレイヤー初期化
	// 自キャラモデル読み込み
	modelPlayer_.reset(Model::CreateFromOBJ("cube", true));
	modelSkydome_ = Model::CreateFromOBJ("sky", true);

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_);
	
	// 自キャラモデル配列
	std::vector<Model*> playerModels = {
	    modelPlayer_.get(),
	};
	// プレイヤー初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

#pragma endregion

	#pragma region 障害物

	/// 箱モデル読み込み
	evilSpiritModel_ = (Model::CreateFromOBJ("Box", true));
	// ボックスのCSVファイル読み込み
	LoadEvilSpiritPopData();
#pragma endregion 

	#pragma region カメラ
	// レールカメラ初期化
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	followCamera_->SetTarget(player_->GetWorldTransformPtr());
	// 追従対象をプレイヤーに
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	
	// デバックカメラ初期化
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

#pragma endregion


}

void GameScene::Update() { 
	
	player_->Update(); 
	skydome_->Update();
	for (const std::unique_ptr<EvilSpirit>& evilSpirit_ : evilSpirits_) {
		evilSpirit_->Update();
	}
	// カメラ処理
	if (isDebugcameraActive_ == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	followCamera_->PlayerSetTransformRotation(player_->GetTransformRotation());
	followCamera_->PlayerSetTransformPosition(player_->GetTransformPosition());


	#pragma region プレイヤーの当たり判定

	PlayerBackZ_ = player_->GetWorldPosition().z - 1.0f;
	PlayerFlontZ_ = player_->GetWorldPosition().z + 1.0f;
	PlayerLeftX_ = player_->GetWorldPosition().x - 1.0f;
	PlayerRightX_ = player_->GetWorldPosition().x + 1.0f;

#pragma endregion


	#pragma region プレイヤーとボックスの当たり判定

	for (const std::unique_ptr<EvilSpirit>& evilSpirit : evilSpirits_) {

		bool boxMoveFlag = evilSpirit->IsDead();

		EvilSpiritFlontZ_ = evilSpirit->GetWorldPosition().z - 1.0f;
		EvilSpiritBackZ_ = evilSpirit->GetWorldPosition().z + 1.0f;
		EvilSpiritRightX_ = evilSpirit->GetWorldPosition().x - 1.0f;
		EvilSpiritLeftX_ = evilSpirit->GetWorldPosition().x + 1.0f;

		if ((PlayerLeftX_ < EvilSpiritRightX_ && PlayerRightX_ > EvilSpiritLeftX_) &&
		    (EvilSpiritFlontZ_ > PlayerBackZ_ && EvilSpiritBackZ_ < PlayerFlontZ_)) {

			boxMoveFlag = true;

			if (boxMoveFlag) {

				Vector3 tmpTranslate = evilSpirit->GetWorldPosition();

				tmpTranslate.y += 7.0f;

				evilSpirit->SetTranslate(tmpTranslate);
				evilSpirit->SetBoxFlag(boxMoveFlag);
			}
		}
		#ifdef _DEBUG 
			// デバッグ
			ImGui::Begin("Debug");
			ImGui::Checkbox("Game boxMoveFlag", &boxMoveFlag);
			ImGui::End();
		#endif
	}

#pragma endregion

	// ボックスのCSVファイルの更新処理
	UpdateEvilSpiritPopCommands();

	// デスフラグの立った敵を削除
	evilSpirits_.remove_if([](std::unique_ptr<EvilSpirit>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});
}

void GameScene::Draw() {

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
	player_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);

	for (const std::unique_ptr<EvilSpirit>& evilSpirit_ : evilSpirits_) {
		evilSpirit_->Draw(viewProjection_);
	}
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


#pragma region 悪霊 CSV

void GameScene::LoadEvilSpiritPopData() {
	evilSpiritPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/BoxPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	evilSpiritPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEvilSpiritPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(evilSpiritPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;
		// 　,区切りで行の先頭文字列を所得

		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			EvilSpiritGenerate({x, y, z});
		}
	}
}

void GameScene::EvilSpiritGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	EvilSpirit* evilSpirit_ = new EvilSpirit();
	evilSpirit_->Initialize(evilSpiritModel_, position);
	evilSpirits_.push_back(static_cast<std::unique_ptr<EvilSpirit>>(evilSpirit_));
}

#pragma endregion