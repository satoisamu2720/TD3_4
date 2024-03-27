#include "SunnyStage.h"
#include "ImGuiManager.h"

void SunnyStage::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();

	texHandle_ = TextureManager::Load("Box/Tex.png");

#pragma region プレイヤー初期化
	// 自キャラモデル読み込み
	modelPlayerBody_.reset(Model::CreateFromOBJ("player_Body", true));
	modelPlayerFront_.reset(Model::CreateFromOBJ("player_Front", true));
	modelPlayerBack_.reset(Model::CreateFromOBJ("player_Back", true));

	// 自キャラモデル配列
	std::vector<Model*> playerModels = {
	    modelPlayerBody_.get(),
	    modelPlayerFront_.get(),
	    modelPlayerBack_.get(),
	};
	// プレイヤー初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
#pragma endregion

#pragma region 障害物

	// 箱モデル読み込み
	BoxModel_ = (Model::CreateFromOBJ("woodenBox", true));
	// ボックスのCSVファイル読み込み
	LoadBoxPopData();

	// 加速装置モデル読み込み
	acceleratorModel_ = (Model::CreateFromOBJ("SpeedUP", true));
	// 加速装置のCSVファイル読み込み
	LoadAcceleratorPopData();

#pragma endregion

#pragma region ステージ
	// ステージ外モデル読み込み
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelStartSkydome_ = Model::CreateFromOBJ("StartSkydome", true);
	modelMiddleSkydome_ = Model::CreateFromOBJ("MiddleSkydome", true);
	
	LoadSkydomePopData();

	// ステージ地面モデル読み込み
	modelGround_ = Model::CreateFromOBJ("ground", true);
	// 地面モデル初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_, {0.0f, -6.0f, 0.0f});

#pragma endregion

#pragma region カメラ
	// レールカメラ初期化
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({0.0f, 4.0f, 10.0f}, {0.0f, 0.0f, 0.0f});
	railCamera_->SetTarget(&player_->GetWorldTransform());
	// 追従対象をプレイヤーに
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetViewProjection(&railCamera_->GetViewProjection());

#pragma endregion
	viewProjection_.farZ = 200.0f;
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void SunnyStage::Update() {

#pragma region 更新処理

	if (player_->GetWeather() == 0) {
		player_->Update();
		player_->SunnyUpdate();
		if (railCamera_->GetStart() == false) {
			railCamera_->SetStart(start);
		}
	}
	if (player_->GetWeather() == 1) {
		player_->Update();
		player_->ThunderstormUpdate();
		if (railCamera_->GetStart() == false) {
			railCamera_->SetStart(start);
		}
	}
	player_->SetWeather(weather);
	for (const std::unique_ptr<Box>& box_ : boxs_) {
		box_->Update();
	}
	// 加速装置
	for (const std::unique_ptr<Accelerator>& accelerator_ : accelerators_) {
		accelerator_->Update();
	}

	for (const std::unique_ptr<Skydome>& MiddleSkydome_ : MiddleSkydomes_) {
		MiddleSkydome_->Update();
	}

	ground_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = SELECT;
	}
#pragma endregion

#pragma region カメラセット
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
#pragma endregion

#ifdef _DEBUG
	ImGui::Begin("weather");
	ImGui::InputFloat("weather", &weather, 1.0f);
	// ImGui::Checkbox("", &);
	ImGui::End();

	ImGui::Begin("stage");
	ImGui::Text("SunnyStage");
	ImGui::Checkbox("Game Start", &start);
	ImGui::End();

	ImGui::Begin("Collision ");
	ImGui::InputFloat("PlayerFlontZSize_", &PlayerFlontZHit_, 0.1f);
	ImGui::InputFloat("PlayerBackZSize_", &PlayerBackZHit_, 0.1f);
	ImGui::InputFloat("PlayerRightXSize_", &PlayerRightXHit_, 0.1f);
	ImGui::InputFloat("PlayerLeftXSize_", &PlayerLeftXHit_, 0.1f);
	ImGui::End();
#endif

	// 当たり判定

#pragma region プレイヤーの当たり判定

	PlayerBackZ_ = player_->GetWorldPosition().z - PlayerBackZHit_;
	PlayerFlontZ_ = player_->GetWorldPosition().z + PlayerFlontZHit_;
	PlayerLeftX_ = player_->GetWorldPosition().x - PlayerLeftXHit_;
	PlayerRightX_ = player_->GetWorldPosition().x + PlayerRightXHit_;

#pragma endregion

#pragma region プレイヤーとボックスの当たり判定

	for (const std::unique_ptr<Box>& box_ : boxs_) {
		BoxBackZ_ = box_->GetWorldPosition().z - 1.0f;
		BoxFlontZ_ = box_->GetWorldPosition().z + 1.0f;
		BoxLeftX_ = box_->GetWorldPosition().x - 1.0f;
		BoxRightX_ = box_->GetWorldPosition().x + 1.0f;

		if ((PlayerLeftX_ < BoxRightX_ && PlayerRightX_ > BoxLeftX_) &&
		    (BoxFlontZ_ > PlayerBackZ_ && BoxBackZ_ < PlayerFlontZ_)) {
			if (timerFlag == false) {
				player_->SetNormalHit(true);
				player_->SetThunderHit(true);
				railCamera_->SetIsSpeedDown(true);
				timerFlag = true;
			}
		}
	}
#pragma endregion

#pragma region プレイヤーと加速装置の当たり判定
	// 加速装置

	for (const std::unique_ptr<Accelerator>& accelerator_ : accelerators_) {
		SpeedBackZ_ = accelerator_->GetWorldPosition().z - 1.0f;
		SpeedFlontZ_ = accelerator_->GetWorldPosition().z + 1.0f;
		SpeedLeftX_ = accelerator_->GetWorldPosition().x - 5.0f;
		SpeedRightX_ = accelerator_->GetWorldPosition().x + 5.0f;

		if ((PlayerLeftX_ < SpeedRightX_ && PlayerRightX_ > SpeedLeftX_) &&
		    (SpeedFlontZ_ > PlayerBackZ_ && SpeedBackZ_ < PlayerFlontZ_)) {

			railCamera_->SetIsSpeedUp(true);

			// player_->SetPosition({0.0f, 0.0f, -50.0f});
		}
	}

#pragma endregion

#pragma region CSV 更新処理,デスフラグ
	// デスフラグの立った敵を削除
	boxs_.remove_if([](std::unique_ptr<Box>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	// ボックスのCSVファイルの更新処理
	UpdateBoxPopCommands();

	// デスフラグの立った敵を削除
	accelerators_.remove_if([](std::unique_ptr<Accelerator>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	// 加速装置のCSVファイルの更新処理
	UpdateAcceleratorPopCommands();

	// デスフラグの立った敵を削除
	MiddleSkydomes_.remove_if([](std::unique_ptr<Skydome>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	// 加速装置のCSVファイルの更新処理
	UpdateSkydomePopCommands();

#pragma endregion 

	Time();
}

void SunnyStage::Draw() { // コマンドリストの取得
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
	for (const std::unique_ptr<Skydome>& MiddleSkydome_ : MiddleSkydomes_) {
		MiddleSkydome_->Draw(viewProjection_);
	}
	//ground_->Draw(viewProjection_);

	for (const std::unique_ptr<Box>& box_ : boxs_) {
		box_->Draw(viewProjection_);
	}

	// 加速装置
	for (const std::unique_ptr<Accelerator>& accelerator_ : accelerators_) {
		accelerator_->Draw(viewProjection_);
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

void SunnyStage::Time() {
	if (timerFlag == true) {
		timer++;
	}
	if (timer >= 120) {
		timer = 0;
		timerFlag = false;
	}
}

#pragma region  ボックス CSV

void SunnyStage::LoadBoxPopData() {
	boxPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/BoxPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	boxPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateBoxPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(boxPopCommands, line)) {
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

			BoxGenerate({x, y, z});
		}
	}
}

void SunnyStage::BoxGenerate(Vector3 position) { 

	// アイテムの生成と初期化処理
	Box* box_ = new Box();
	box_->Initialize(BoxModel_, position);
	boxs_.push_back(static_cast<std::unique_ptr<Box>>(box_));
}

#pragma endregion

#pragma region 加速装置 CSV

void SunnyStage::LoadAcceleratorPopData() {
	acceleratorPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/AcceleratorPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	acceleratorPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateAcceleratorPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(acceleratorPopCommands, line)) {
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

			AcceleratorGenerate({x, y, z});
		}
	}
}
void SunnyStage::AcceleratorGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Accelerator* accelerator_ = new Accelerator();
	accelerator_->Initialize(acceleratorModel_, position);
	accelerators_.push_back(static_cast<std::unique_ptr<Accelerator>>(accelerator_));
}

#pragma endregion

#pragma region 背景 CSV

void SunnyStage::LoadSkydomePopData() {
	MiddlekydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/MiddleSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	MiddlekydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateSkydomePopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(MiddlekydomePopCommands, line)) {
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

			SkydomeGenerate({x, y, z});
		}
	}
}

void SunnyStage::SkydomeGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Skydome* MiddleSkydome_ = new Skydome();
	MiddleSkydome_->Initialize(modelMiddleSkydome_, position);
	MiddleSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(MiddleSkydome_));
}

#pragma endregion