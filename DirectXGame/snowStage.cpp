#include "SnowStage.h"
void SnowStage::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	light_ = LightGroup::Create();

	texHandle_ = TextureManager::Load("Box/Tex.png");

#pragma region タイム

	timer_ = std::make_unique<Timer>();
	textureHandleNumber_ = TextureManager::Load("number.png");

	for (int i = 0; i < 2; i++) {
		spriteSecondTime_[i] = Sprite::Create(textureHandleNumber_, {0.0f + i * 26, 10});
		// spriteSecondTime_[i] = Sprite::Create(textureHandleNumber_, {60.0f + i * 26, 10});
	}
	timer_->SetTime(0, 30);

#pragma endregion

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

	// ゴミ箱モデル読み込み
	modelGarbageCan_ = (Model::CreateFromOBJ("GarbageCan", true));
	// ゴミ箱モデル初期化
	garbageCan_ = std::make_unique<GarbageCan>();
	garbageCan_->Initialize(modelGarbageCan_, {-16.0f, 1.5f, 50.0f});

#pragma endregion

#pragma region ステージ
	// ステージ外モデル読み込み
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelStartSkydome_ = Model::CreateFromOBJ("StartSkydome", true);
	modelMiddleSkydome_ = Model::CreateFromOBJ("MiddleSkydome", true);
	modelGoalSkydome_ = Model::CreateFromOBJ("GoalSkydome", true);

	LoadMiddleSkydomePopData();

	LoadStartSkydomePopData();

	LoadGoalSkydomePopData();

	// ステージ地面モデル読み込み
	modelGround_ = Model::CreateFromOBJ("ground", true);
	// 地面モデル初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_, {0.0f, -6.0f, 0.0f});

	// ガードレール
	modelGuardRail_ = Model::CreateFromOBJ("guardRail", true);

	LoadGuardRailPopData();

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

#pragma region 雪

	modelRaindrop_ = Model::CreateFromOBJ("raindrop", true);

	// 雪の初期化
	for (int i = 0; i < 100; i++) {
		worldTransformSnow_[i].Initialize();
		snowCheck_[i] = false;
	}

	snowDownSpeed = 0;

#pragma endregion

	viewProjection_.farZ = 200.0f;
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void SnowStage::Update() {

#pragma region 雪

	// 雪の更新
	for (int i = 0; i < 100; i++) {
		if (snowCheck_[i] == false) {
			snowCheck_[i] = true;
			/*worldTransformSnow_->translation_.x = ;
			worldTransformSnow_->translation_.y = ;*/
			worldTransformSnow_[i].translation_.z =
			    player_->GetWorldTransform().translation_.z + 40.0f;
		}

		if (snowCheck_[i] == true) {
			worldTransformSnow_[i].translation_.y += snowDownSpeed;
		}

		if (worldTransformSnow_[i].translation_.y >= 720) {
			worldTransformSnow_[i].translation_.y = 0;
			snowCheck_[i] = false;
		}

	}

#pragma endregion

#pragma region 更新処理

	timer_->Update();

	player_->Update();

	player_->SetWeather(weather);

	garbageCan_->Update();

	for (const std::unique_ptr<Box>& box_ : boxs_) {
		box_->Update();
	}
	// 加速装置
	for (const std::unique_ptr<Accelerator>& accelerator_ : accelerators_) {
		accelerator_->Update();
	}

	for (const std::unique_ptr<Skydome>& MiddleSkydome_ : middleSkydomes_) {
		MiddleSkydome_->Update();
	}

	for (const std::unique_ptr<Skydome>& goalSkydome_ : goalSkydomes_) {
		goalSkydome_->Update();
	}
	for (const std::unique_ptr<GuardRail>& guardRail_ : guardRails_) {
		guardRail_->Update();
	}

	ground_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
		Reset();
	}

	if (input_->TriggerKey(DIK_LSHIFT) && start == false) {
		start = true;
		railCamera_->SetStart(start);
		timer_->SetTimerFlag(true);
	} else if (input_->TriggerKey(DIK_LSHIFT) && start == true) {
		start = false;
		railCamera_->SetStart(start);
	}

	if (input_->TriggerKey(DIK_R)) {
		timer_->SetTimerFlag(false);
		railCamera_->SetStart(false);
		timer_->SetTime(0, 30);
		railCamera_->SetPos({0, 4, 0});
		garbageCan_->SetPlayerGetPos({railCamera_->GetWorldTransform().translation_});
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
	ImGui::Text("Rain");
	ImGui::Checkbox("Game Start", &start);
	ImGui::End();

	ImGui::Begin("Collision ");
	ImGui::InputFloat("PlayerFlontZSize_", &PlayerFlontZHit_, 0.1f);
	ImGui::InputFloat("PlayerBackZSize_", &PlayerBackZHit_, 0.1f);
	ImGui::InputFloat("PlayerRightXSize_", &PlayerRightXHit_, 0.1f);
	ImGui::InputFloat("PlayerLeftXSize_", &PlayerLeftXHit_, 0.1f);
	ImGui::End();

	ImGui::Begin("Clear ");
	ImGui::Checkbox("clearFlag", &goalTimerFlag);
	ImGui::InputFloat("clearTimer", &goalTimer, 0.1f);
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

	for (const std::unique_ptr<Box>& box : boxs_) {

		bool boxMoveFlag = box->IsDead();

		BoxBackZ_ = box->GetWorldPosition().z - 1.0f;
		BoxFlontZ_ = box->GetWorldPosition().z + 1.0f;
		BoxLeftX_ = box->GetWorldPosition().x - 1.0f;
		BoxRightX_ = box->GetWorldPosition().x + 1.0f;

		if ((PlayerLeftX_ < BoxRightX_ && PlayerRightX_ > BoxLeftX_) &&
		    (BoxFlontZ_ > PlayerBackZ_ && BoxBackZ_ < PlayerFlontZ_)) {

			boxMoveFlag = true;

			if (boxMoveFlag) {

				Vector3 tmpTranslate = box->GetWorldPosition();

				tmpTranslate.y += 7.0f;

				if (timerFlag == false) {
					player_->SetNormalHit(true);
					// player_->SetThunderHit(true);
					railCamera_->SetIsSpeedDown(true);
					timerFlag = true;
				}

				box->SetTranslate(tmpTranslate);
				box->SetBoxFlag(boxMoveFlag);
			}
		}
	}

#pragma endregion

#pragma region プレイヤーとゴミ箱の当たり判定

	bool garbageCanMoveFlag = garbageCan_->IsDead();

	GarbageCanFlontZ_ = garbageCan_->GetWorldPosition().z + FlontZHit_;
	GarbageCanBackZ_ = garbageCan_->GetWorldPosition().z - BackZHit_;
	GarbageCanRightX_ = garbageCan_->GetWorldPosition().x + RightXHit_;
	GarbageCanLeftX_ = garbageCan_->GetWorldPosition().x - LeftXHit_;

	if ((PlayerLeftX_ < GarbageCanRightX_ && PlayerRightX_ > GarbageCanLeftX_) &&
	    (GarbageCanFlontZ_ > PlayerBackZ_ && GarbageCanBackZ_ < PlayerFlontZ_)) {

		garbageCanMoveFlag = true;

		if (garbageCanMoveFlag) {

			// Vector3 tmpTranslate = garbageCan_->GetWorldPosition();

			// tmpTranslate.x += 7.0f;

			if (timerFlag == false) {
				player_->SetNormalHit(true);
				railCamera_->SetIsSpeedDown(true);
				garbageCan_->SetRotate(true);
				timerFlag = true;
			}

			// garbageCan_->SetTranslate(tmpTranslate);
			// garbageCan_->SetGarbageCanFlag(garbageCanMoveFlag);
		}
	}

	if (garbageCan_->GetHit() == true) {
		garbageCan_->SetPlayerGetPos({railCamera_->GetWorldTransform().translation_});
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
		}
	}

#pragma endregion

#pragma region プレイヤーとゴールの当たり判定

	for (const std::unique_ptr<Skydome>& goalSkydome_ : goalSkydomes_) {
		goalBackZ_ = goalSkydome_->GetWorldPosition().z + 5.0f;
		goalFlontZ_ = goalSkydome_->GetWorldPosition().z + 5.0f;
		goalLeftX_ = goalSkydome_->GetWorldPosition().x - 10.0f;
		goalRightX_ = goalSkydome_->GetWorldPosition().x + 10.0f;

		if ((PlayerLeftX_ < goalRightX_ && PlayerRightX_ > goalLeftX_) &&
		    (goalFlontZ_ > PlayerBackZ_ && goalBackZ_ < PlayerFlontZ_)) {

			goalTimerFlag = true;
		}
	}
#pragma endregion

#pragma region CSV 更新処理, デスフラグ
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
	middleSkydomes_.remove_if([](std::unique_ptr<Skydome>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	// 装置のCSVファイルの更新処理
	UpdateMiddleSkydomePopCommands();

	startSkydomes_.remove_if([](std::unique_ptr<Skydome>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	UpdateStartSkydomePopCommands();

	goalSkydomes_.remove_if([](std::unique_ptr<Skydome>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	UpdateGoalSkydomePopCommands();

	guardRails_.remove_if([](std::unique_ptr<GuardRail>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	UpdateGuardRailPopCommands();

#pragma endregion

	Time();
	Goal();
}

#pragma region タイム

void SnowStage::DrawTime() {

	////分数
	// int eachMathNumber[2] = {};
	// int mathNumber = timer_->GetTimeMath();
	// int mathKeta = 10;
	// for (int i = 0; i < 2; i++) {
	//	eachMathNumber[i] = mathNumber / mathKeta;
	//	mathNumber = mathNumber % mathKeta;
	//	mathKeta = mathKeta / 10;
	// }
	// 秒数
	int eachSecondNumber[2] = {};
	int secondNumber = timer_->GetTimeSecond();
	int secondKeta = 10;
	for (int i = 0; i < 2; i++) {
		eachSecondNumber[i] = secondNumber / secondKeta;
		secondNumber = secondNumber % secondKeta;
		secondKeta = secondKeta / 10;
	}

	for (int i = 0; i < 2; i++) {
		spriteSecondTime_[i]->SetSize({32, 64});
		spriteSecondTime_[i]->SetTextureRect({32.0f * eachSecondNumber[i], 0}, {32, 64});
		spriteSecondTime_[i]->Draw();

		/*spriteMathTime_[i]->SetSize({32, 64});
		spriteMathTime_[i]->SetTextureRect({32.0f * eachMathNumber[i], 0}, {32, 64});
		spriteMathTime_[i]->Draw();*/
	}
}

#pragma endregion

void SnowStage::Draw() { // コマンドリストの取得
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

	for (int i = 0; i < 100; i++) {
		modelRaindrop_->Draw(worldTransformSnow_[i], viewProjection_);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	}

	// garbageCan_->Draw(viewProjection_);

	// for (const std::unique_ptr<Skydome>& startSkydome_ : startSkydomes_) {
	//	startSkydome_->Draw(viewProjection_);
	// }
	// for (const std::unique_ptr<Skydome>& MiddleSkydome_ : middleSkydomes_) {
	//	MiddleSkydome_->Draw(viewProjection_);
	// }
	// for (const std::unique_ptr<Skydome>& goalSkydome_ : goalSkydomes_) {
	//	goalSkydome_->Draw(viewProjection_);
	// }
	//// ground_->Draw(viewProjection_);

	// for (const std::unique_ptr<Box>& box_ : boxs_) {
	//	box_->Draw(viewProjection_);
	// }

	//// 加速装置
	// for (const std::unique_ptr<Accelerator>& accelerator_ : accelerators_) {
	//	accelerator_->Draw(viewProjection_);
	// }
	/*for (const std::unique_ptr<GuardRail>& guardRail_ : guardRails_) {
	    guardRail_->Draw(viewProjection_);
	}*/

	Model::PostDraw();

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	DrawTime();
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
}

void SnowStage::Time() {
	if (timerFlag == true) {
		timer++;
	}
	if (timer >= 30) {
		timer = 0;
		timerFlag = false;
	}
}

#pragma region ボックス CSV

void SnowStage::LoadBoxPopData() {
	boxPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/BoxPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	boxPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SnowStage::UpdateBoxPopCommands() {
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

void SnowStage::BoxGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	Box* box_ = new Box();
	box_->Initialize(BoxModel_, position);
	boxs_.push_back(static_cast<std::unique_ptr<Box>>(box_));
}

#pragma endregion

#pragma region 加速装置 CSV

void SnowStage::LoadAcceleratorPopData() {
	acceleratorPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/AcceleratorPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	acceleratorPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SnowStage::UpdateAcceleratorPopCommands() {
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

void SnowStage::AcceleratorGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Accelerator* accelerator_ = new Accelerator();
	accelerator_->Initialize(acceleratorModel_, position);
	accelerators_.push_back(static_cast<std::unique_ptr<Accelerator>>(accelerator_));
}

#pragma endregion

#pragma region 開始背景 CSV

void SnowStage::LoadStartSkydomePopData() {
	startSkydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/StartSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	startSkydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SnowStage::UpdateStartSkydomePopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(startSkydomePopCommands, line)) {
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

			StartSkydomeGenerate({x, y, z});
		}
	}
}

void SnowStage::StartSkydomeGenerate(Vector3 position) { // アイテムの生成と初期化処理
	Skydome* startSkydome_ = new Skydome();
	startSkydome_->Initialize(modelStartSkydome_, position);
	startSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(startSkydome_));
}

#pragma endregion

#pragma region 直線背景 CSV

void SnowStage::LoadMiddleSkydomePopData() {
	middleSkydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/MiddleSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	middleSkydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SnowStage::UpdateMiddleSkydomePopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(middleSkydomePopCommands, line)) {
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

			MiddleSkydomeGenerate({x, y, z});
		}
	}
}

void SnowStage::MiddleSkydomeGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Skydome* middleSkydome_ = new Skydome();
	middleSkydome_->Initialize(modelMiddleSkydome_, position);
	middleSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(middleSkydome_));
}

#pragma endregion

#pragma region ゴール背景 CSV

void SnowStage::LoadGoalSkydomePopData() {

	goalSkydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/GoalSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	goalSkydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SnowStage::UpdateGoalSkydomePopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(goalSkydomePopCommands, line)) {
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

			GoalSkydomeGenerate({x, y, z});
		}
	}
}

void SnowStage::GoalSkydomeGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Skydome* goalSkydome_ = new Skydome();
	goalSkydome_->Initialize(modelGoalSkydome_, position);
	goalSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(goalSkydome_));
}

#pragma endregion

#pragma region ガードレール CSV

void SnowStage::LoadGuardRailPopData() {

	guardRailPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/GuardRailPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	guardRailPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SnowStage::UpdateGuardRailPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(guardRailPopCommands, line)) {
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

			GuardRailGenerate({x, y, z});
		}
	}
}

void SnowStage::GuardRailGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	GuardRail* guardRail_ = new GuardRail();
	guardRail_->Initialize(modelGuardRail_, position);
	guardRails_.push_back(static_cast<std::unique_ptr<GuardRail>>(guardRail_));
}

#pragma endregion

void SnowStage::Reset() {
	boxs_.clear();
	accelerators_.clear();
	startSkydomes_.clear();
	middleSkydomes_.clear();
	goalSkydomes_.clear();
	sceneNo = SELECT;
}

void SnowStage::Goal() {

	if (goalTimerFlag == true) {
		goalTimer++;
	}
	if (goalTimer >= 60) {
		boxs_.clear();
		accelerators_.clear();
		startSkydomes_.clear();
		middleSkydomes_.clear();
		goalSkydomes_.clear();
		guardRails_.clear();
		goalTimer = 0;
		goalTimerFlag = false;

		if (timer_->GetTimeSecond() > 0) {
			timer_->SetTimerFlag(false);
			railCamera_->SetStart(false);
			timer_->SetTime(0, 30);
			railCamera_->SetPos({0, 4, 0});
			sceneNo = CLEAR;
		} else {
			timer_->SetTimerFlag(false);
			railCamera_->SetStart(false);
			timer_->SetTime(0, 30);
			railCamera_->SetPos({0, 4, 0});
			sceneNo = END;
		}
	}
}
