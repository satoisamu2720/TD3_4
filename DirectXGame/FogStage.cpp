#include "scene/Fogstage.h"

void FogStage::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	light_ = LightGroup::Create();

	pos = {0, -300};

	Vector2 scale = {1180, 600};
	Vector4 Color = {1, 1, 1, 0.9f};

	fogTexture_ = TextureManager::Load("th.png");
	// BlendTexture_ = TextureManager::Load("Blend.png");

	fogsprite_ = Sprite::Create(fogTexture_, pos, Color);

	// fogsprite_->SetSize(scale);

	BlendSprite_ = Sprite::Create(BlendTexture_, pos, Color);


	


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

	viewProjection_.farZ = 200.0f;
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void FogStage::Update() {

#pragma region 更新処理

	timer_->Update();

	player_->Update();

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
	/*for (const std::unique_ptr<GuardRail>& guardRail_ : guardRails_) {
		guardRail_->Update();
	}*/

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
	}

#pragma endregion

#pragma region カメラセット
	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
#pragma endregion

#ifdef _DEBUG

	ImGui::Begin("stage");
	ImGui::Text("SunnyStage");
	ImGui::Checkbox("Game Start", &start);
	ImGui::End();

	ImGui::Begin("Collision ");
	ImGui::InputFloat("PlayerFlontZSize_", &FlontZHit_, 0.1f);
	ImGui::InputFloat("PlayerBackZSize_", &BackZHit_, 0.1f);
	ImGui::InputFloat("PlayerRightXSize_", &RightXHit_, 0.1f);
	ImGui::InputFloat("PlayerLeftXSize_", &LeftXHit_, 0.1f);
	ImGui::End();

	ImGui::Begin("Clear ");
	ImGui::Checkbox("clearFlag", &goalTimerFlag);
	ImGui::InputFloat("clearTimer", &goalTimer, 0.1f);
	ImGui::End();

#endif

	// 当たり判定

#pragma region プレイヤーの当たり判定

	PlayerBackZ_ = player_->GetWorldPosition().z - 2.4f;
	PlayerFlontZ_ = player_->GetWorldPosition().z + 2.3f;
	PlayerLeftX_ = player_->GetWorldPosition().x - 1.3f;
	PlayerRightX_ = player_->GetWorldPosition().x + 1.3f;

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

	/*pos.x = Pos_.translation_.x;
	pos.y = Pos_.translation_.y;*/

	/*if (Pos_.translation_.x<=4500.0f)
	{
	    Pos_.translation_.x += 10.0f;
	}
	else
	{
	    Pos_.translation_.x -= 2.0f;
	}

	Pos_.UpdateMatrix();*/

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

	/*guardRails_.remove_if([](std::unique_ptr<GuardRail>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});*/

	UpdateGuardRailPopCommands();

#pragma endregion

	Time();
	Goal();
}

#pragma region タイム

void FogStage::DrawTime() {

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

void FogStage::Draw() { // コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// BlendSprite_->Draw();

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

	for (const std::unique_ptr<Skydome>& startSkydome_ : startSkydomes_) {
		startSkydome_->Draw(viewProjection_);
	}
	for (const std::unique_ptr<Skydome>& MiddleSkydome_ : middleSkydomes_) {
		MiddleSkydome_->Draw(viewProjection_);
	}
	for (const std::unique_ptr<Skydome>& goalSkydome_ : goalSkydomes_) {
		goalSkydome_->Draw(viewProjection_);
	}
	// ground_->Draw(viewProjection_);

	for (const std::unique_ptr<Box>& box_ : boxs_) {
		box_->Draw(viewProjection_);
	}

	// 加速装置
	for (const std::unique_ptr<Accelerator>& accelerator_ : accelerators_) {
		accelerator_->Draw(viewProjection_);
	}
	/*for (const std::unique_ptr<GuardRail>& guardRail_ : guardRails_) {
	    guardRail_->Draw(viewProjection_);
	}*/

	Model::PostDraw();

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	///
	///
	Sprite::BlendMode::kNone;

	fogsprite_->Draw();

	DrawTime();
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
}

void FogStage::Time() {
	if (timerFlag == true) {
		timer++;
	}
	if (timer >= 30) {
		timer = 0;
		timerFlag = false;
	}
}

#pragma region ボックス CSV

void FogStage::LoadBoxPopData() {
	boxPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/BoxPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	boxPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void FogStage::UpdateBoxPopCommands() {
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

void FogStage::BoxGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	Box* box_ = new Box();
	box_->Initialize(BoxModel_, position);
	boxs_.push_back(static_cast<std::unique_ptr<Box>>(box_));
}

#pragma endregion

#pragma region 加速装置 CSV

void FogStage::LoadAcceleratorPopData() {
	acceleratorPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/AcceleratorPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	acceleratorPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void FogStage::UpdateAcceleratorPopCommands() {
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

void FogStage::AcceleratorGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Accelerator* accelerator_ = new Accelerator();
	accelerator_->Initialize(acceleratorModel_, position);
	accelerators_.push_back(static_cast<std::unique_ptr<Accelerator>>(accelerator_));
}

#pragma endregion

#pragma region 開始背景 CSV

void FogStage::LoadStartSkydomePopData() {
	startSkydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/StartSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	startSkydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void FogStage::UpdateStartSkydomePopCommands() {
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

void FogStage::StartSkydomeGenerate(Vector3 position) { // アイテムの生成と初期化処理
	Skydome* startSkydome_ = new Skydome();
	startSkydome_->Initialize(modelStartSkydome_, position);
	startSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(startSkydome_));
}

#pragma endregion

#pragma region 直線背景 CSV

void FogStage::LoadMiddleSkydomePopData() {
	middleSkydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/MiddleSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	middleSkydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void FogStage::UpdateMiddleSkydomePopCommands() {
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

void FogStage::MiddleSkydomeGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Skydome* middleSkydome_ = new Skydome();
	middleSkydome_->Initialize(modelMiddleSkydome_, position);
	middleSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(middleSkydome_));
}

#pragma endregion

#pragma region ゴール背景 CSV

void FogStage::LoadGoalSkydomePopData() {

	goalSkydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/GoalSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	goalSkydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void FogStage::UpdateGoalSkydomePopCommands() {
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

void FogStage::GoalSkydomeGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Skydome* goalSkydome_ = new Skydome();
	goalSkydome_->Initialize(modelGoalSkydome_, position);
	goalSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(goalSkydome_));
}

#pragma endregion

#pragma region ガードレール CSV

void FogStage::LoadGuardRailPopData() {

	guardRailPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/GuardRailPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	guardRailPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void FogStage::UpdateGuardRailPopCommands() {
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

void FogStage::GuardRailGenerate(Vector3 position) {
	//// アイテムの生成と初期化処理
	//GuardRail* guardRail_ = new GuardRail();
	//guardRail_->Initialize(modelGuardRail_, position);
	//guardRails_.push_back(static_cast<std::unique_ptr<GuardRail>>(guardRail_));
}

#pragma endregion

void FogStage::Reset() {
	boxs_.clear();
	accelerators_.clear();
	startSkydomes_.clear();
	middleSkydomes_.clear();
	goalSkydomes_.clear();
	sceneNo = SELECT;
}

void FogStage::Goal() {

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
