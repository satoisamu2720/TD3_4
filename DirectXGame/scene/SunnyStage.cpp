#include "SunnyStage.h"

void SunnyStage::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	light_ = LightGroup::Create();

	
	
	texHandle_ = TextureManager::Load("Box/Tex.png");

#pragma region タイム

	timer_ = std::make_unique<Timer>();
	
	textureHandleNumber_ = TextureManager::Load("number.png");

	for (int i = 0; i < 2; i++) {
		spriteSecondTime_[i] = Sprite::Create(textureHandleNumber_, {10.0f + i * 46, 20});
		spriteStartTime_[i] = Sprite::Create(textureHandleNumber_, {testPosTimer.x + i * 26, testPosTimer.y});
	}
	timer_->SetTime(0, 15);
	timer_->SetStartTimer(4);
#pragma endregion

#pragma region プレイヤー初期化
	// 自キャラモデル読み込み
	modelPlayerBody_.reset(Model::CreateFromOBJ("cube", true));
	// 自キャラモデル配列
	std::vector<Model*> playerModels = {
	    modelPlayerBody_.get(),
	};
	// プレイヤー初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
#pragma endregion

#pragma region 障害物

	// 箱モデル読み込み
	BoxModel_ = (Model::CreateFromOBJ("colorCorn", true));
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

	//ガードレール

	//LoadGuardRailPopData();

	

#pragma endregion

#pragma region カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	followCamera_->SetTarget(player_->GetWorldTransformPtr());
	// 追従対象をプレイヤーに
	player_->SetViewProjection(&followCamera_->GetViewProjection());

#pragma endregion

	viewProjection_.farZ = 200.0f;
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	

	
	//summerSound_ = Audio::GetInstance()->LoadWave("Sound/summer.mp3"); // 晴BGM

	//Audio::GetInstance()->Audio::PlayWave(summerSound_, true, 0.5f);
	//  サウンド

	BGM_ = Audio::GetInstance()->LoadWave("Sound/BGM.mp3");
	cloudSound_ = Audio::GetInstance()->LoadWave("Sound/cloud.mp3");       // 雲
	moveSound_ = Audio::GetInstance()->LoadWave("Sound/button06.mp3");     // ADボタン
	decisionSound_ = Audio::GetInstance()->LoadWave("Sound/button01.mp3"); // 決定ボタン
	summerSound_ = Audio::GetInstance()->LoadWave("Sound/summer.mp3");     // 晴BGM
	gameOverSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameOver.mp3");
	gameClearSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameClear.mp3");
	CarSound_ = Audio::GetInstance()->LoadWave("Sound/Car.mp3"); // 車走行



	Audio::GetInstance()->Audio::PauseWave(summerSound_);
}

void SunnyStage::Update() {

#pragma region 更新処理
	
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
	    for (const std::unique_ptr<TrafficLight>& guardRail_ : trafficLight_) {
			guardRail_->Update();
		}

		//ground_->Update();

		

		if (timer_->GetStartTime() == 3) {
			
	    }
		

		if (timer_->GetStartTime() <= 0 && start == false) {
		    timer_->SetTimerFlag(true);
		    //Audio::GetInstance()->Audio::PlayWave(CarSound_, true, 1.0f);
		}

#pragma endregion

#pragma region カメラセット
		followCamera_->Update();
	    viewProjection_.matView = followCamera_->GetViewProjection().matView;
	    viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

#pragma endregion

		// 当たり判定

#pragma region プレイヤーの当たり判定

		PlayerBackZ_ = player_->GetWorldPosition().z - 1.0f;
		PlayerFlontZ_ = player_->GetWorldPosition().z + 1.0f;
		PlayerLeftX_ = player_->GetWorldPosition().x - 1.0f;
		PlayerRightX_ = player_->GetWorldPosition().x + 1.0f;
	    PlayerDownY_ = player_->GetWorldPosition().y - 1.0f;
	    PlayerUpY_ = player_->GetWorldPosition().y + 1.0f;


#pragma endregion

#pragma region プレイヤーとボックスの当たり判定

		for (const std::unique_ptr<Box>& box : boxs_) {

			bool boxMoveFlag = box->IsDead();

			BoxBackZ_ = box->GetWorldPosition().z - 1.0f;
			BoxFlontZ_ = box->GetWorldPosition().z + 1.0f;
			BoxLeftX_ = box->GetWorldPosition().x - 1.0f;
			BoxRightX_ = box->GetWorldPosition().x + 1.0f;
		    BoxDownY_ = box->GetWorldPosition().y - 1.0f;
		    BoxUpY_ = box->GetWorldPosition().y + 1.0f;

			if ((PlayerLeftX_ < BoxRightX_ && PlayerRightX_ > BoxLeftX_) &&
			    (BoxFlontZ_ > PlayerBackZ_ && BoxBackZ_ < PlayerFlontZ_) &&
				PlayerDownY_< BoxUpY_ && PlayerUpY_ > BoxDownY_) {

				boxMoveFlag = true;

				if (boxMoveFlag) {

					Vector3 tmpTranslate = box->GetWorldPosition();

					tmpTranslate.y += 7.0f;

					if (timerFlag == false) {
						// player_->SetThunderHit(true);
						timerFlag = true;
					}

					box->SetTranslate(tmpTranslate);
					box->SetBoxFlag(boxMoveFlag);
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
				
			}
		}

#pragma endregion

#pragma region プレイヤーとゴールの当たり判定

		for (const std::unique_ptr<Skydome>& goalSkydome_ : goalSkydomes_) {
			goalBackZ_ = goalSkydome_->GetWorldPosition().z + 5.0f;
			goalFlontZ_ = goalSkydome_->GetWorldPosition().z + 5.0f;
		    goalLeftX_ = goalSkydome_->GetWorldPosition().x - 20.0f;
		    goalRightX_ = goalSkydome_->GetWorldPosition().x + 20.0f;

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

		trafficLight_.remove_if([](std::unique_ptr<TrafficLight>& item) {
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
	

#ifdef _DEBUG
	

	if (input_->TriggerKey(DIK_LSHIFT) && start == false) {
		start = true;
		
		timer_->SetTimerFlag(true);
	} else if (input_->TriggerKey(DIK_LSHIFT) && start == true) {
		start = false;
		
	}

	if (input_->TriggerKey(DIK_R)) {
		timer_->SetTimerFlag(false);
		timer_->SetTime(0, 30);
		followCamera_->SetPos({0, 4, 0});
		
	}


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

	ImGui::Begin("start timer ");
	ImGui::DragFloat("start timer pos x", &testPosTimer.x);
	ImGui::DragFloat("start timer pos y", &testPosTimer.y);
	ImGui::End();

#endif

}

#pragma region タイム

void SunnyStage::DrawTime() {

	//ゲームスタートタイマー秒数
	int eachMathNumber[2] = {};
	int mathNumber = timer_->GetStartTime();
	int mathKeta = 10;
	for (int i = 0; i < 2; i++) {
		eachMathNumber[i] = mathNumber / mathKeta;
		mathNumber = mathNumber % mathKeta;
		mathKeta = mathKeta / 10;
	}
	//秒数
	int eachSecondNumber[2] = {};
	int secondNumber = timer_->GetTimeSecond();
	int secondKeta = 10;
	for (int i = 0; i < 2; i++) {
		eachSecondNumber[i] = secondNumber / secondKeta;
		secondNumber = secondNumber % secondKeta;
		secondKeta = secondKeta / 10;
	}

	for (int i = 0; i < 2; i++) {
		//残り時間描画
		spriteSecondTime_[i]->SetSize({64, 128});
		spriteSecondTime_[i]->SetTextureRect({32.0f * eachSecondNumber[i], 0}, {32, 64});
		spriteSecondTime_[i]->Draw();

		//スタート秒数描画
		spriteStartTime_[1]->SetSize({128, 256});
		spriteStartTime_[1]->SetPosition(testPosTimer);
		spriteStartTime_[1]->SetTextureRect({32.0f * eachMathNumber[1], 0}, {32, 64});
		if (start == false) {
		 spriteStartTime_[1]->Draw();
		}
		
	}
}

#pragma endregion

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

	

	for (const std::unique_ptr<Skydome>& startSkydome_ : startSkydomes_) {
		startSkydome_->Draw(viewProjection_);
	}
	for (const std::unique_ptr<Skydome>& MiddleSkydome_ : middleSkydomes_) {
		MiddleSkydome_->Draw(viewProjection_);
	}
	for (const std::unique_ptr<Skydome>& goalSkydome_ : goalSkydomes_) {
		goalSkydome_->Draw(viewProjection_);
	}
	//ground_->Draw(viewProjection_);

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
	DrawTime();
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
}

void SunnyStage::Time() {
	if (timerFlag == true) {
		timer++;
	}
	if (timer >= 30) {
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

#pragma region 開始背景 CSV

void SunnyStage::LoadStartSkydomePopData() {
	startSkydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/StartSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	startSkydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateStartSkydomePopCommands() {
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

void SunnyStage::StartSkydomeGenerate(Vector3 position) {// アイテムの生成と初期化処理
	Skydome* startSkydome_ = new Skydome();
	startSkydome_->Initialize(modelStartSkydome_, position);
	startSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(startSkydome_));
}

#pragma endregion

#pragma region 直線背景 CSV

void SunnyStage::LoadMiddleSkydomePopData() {
	middleSkydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/MiddleSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	middleSkydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateMiddleSkydomePopCommands() {
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

void SunnyStage::MiddleSkydomeGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Skydome* middleSkydome_ = new Skydome();
	middleSkydome_->Initialize(modelMiddleSkydome_, position);
	middleSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(middleSkydome_));
}

#pragma endregion

#pragma region ゴール背景 CSV

void SunnyStage::LoadGoalSkydomePopData() {

	goalSkydomePopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/GoalSkydomePop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	goalSkydomePopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateGoalSkydomePopCommands() {
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

void SunnyStage::GoalSkydomeGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Skydome* goalSkydome_ = new Skydome();
	goalSkydome_->Initialize(modelGoalSkydome_, position);
	goalSkydomes_.push_back(static_cast<std::unique_ptr<Skydome>>(goalSkydome_));
}

#pragma endregion

#pragma region ガードレール CSV

void SunnyStage::LoadGuardRailPopData() {

	trafficLightPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/GuardRailPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	trafficLightPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateGuardRailPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(trafficLightPopCommands, line)) {
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

			trafficLight({x, y, z});
		}
	}
}

void SunnyStage::trafficLight(Vector3 position) {
	// アイテムの生成と初期化処理
	TrafficLight* trafficLight = new TrafficLight();
	trafficLight->Initialize(modelGuardRail_, position);
	trafficLight_.push_back(static_cast<std::unique_ptr<TrafficLight>>(trafficLight));
}

#pragma endregion


void SunnyStage::Reset() { 
	boxs_.clear();
	accelerators_.clear();
	startSkydomes_.clear();
	middleSkydomes_.clear();
	goalSkydomes_.clear();
	
	timer_->SetTime(0, 30);
	timer_->SetTimerFlag(false);
	followCamera_->SetPos({0, 4, 0});
	
	start = false;

	sceneNo = SELECT;
}

void SunnyStage::Goal() {

	if (goalTimerFlag == true) {
		goalTimer++;
	}
	if (goalTimer >= 60) {
		boxs_.clear();
		accelerators_.clear();
		startSkydomes_.clear();
		middleSkydomes_.clear();
		goalSkydomes_.clear();
		trafficLight_.clear();
		goalTimer = 0;
		goalTimerFlag = false;
		start = false;

		if (timer_->GetTimeSecond() > 0) {

			//Audio::GetInstance()->Audio::StopWave(summerSound_);
			//Audio::GetInstance()->Audio::StopWave(CarSound_);
			
			timer_->SetTime(0, 30); 
			timer_->SetTimerFlag(false);
			followCamera_->SetPos({0, 4, 0});
			
			sceneNo = CLEAR;
		} else {
			//Audio::GetInstance()->Audio::StopWave(summerSound_);
			//Audio::GetInstance()->Audio::StopWave(CarSound_);

			timer_->SetTime(0, 30);
			timer_->SetTimerFlag(false);
			followCamera_->SetPos({0, 4, 0});
			
			sceneNo = END;
		}
	}
}
 
