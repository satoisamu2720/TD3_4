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
		spriteStartTime_[i] =
		    Sprite::Create(textureHandleNumber_, {testPosTimer.x + i * 26, testPosTimer.y});
	}
	timer_->SetTime(0, 30);
	timer_->SetStartTimer(4);
#pragma endregion

#pragma region プレイヤー初期化
	// 自キャラモデル読み込み
	modelPlayerBody_.reset(Model::CreateFromOBJ("player", true));
	modelPlayerLight_.reset(Model::CreateFromOBJ("light", true));

	// 自キャラモデル配列
	std::vector<Model*> playerModels = {
	    modelPlayerBody_.get(),
	    modelPlayerLight_.get(),
	};
	// プレイヤー初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

	

#pragma endregion

#pragma region 障害物

	// 箱モデル読み込み
	BoxModel_ = (Model::CreateFromOBJ("block_Blue", true));
	// ボックスのCSVファイル読み込み
	LoadBoxPopData();

	BlockModel_ = (Model::CreateFromOBJ("block_WB", true));
	//CSVファイル読み込み
	LoadInvisibleBoxPopData();

	// 加速装置モデル読み込み
	acceleratorModel_ = (Model::CreateFromOBJ("enemy", true));
	// 加速装置のCSVファイル読み込み
	LoadAcceleratorPopData();

#pragma endregion

#pragma region オブジェクト

	// 　悪霊モデル読み込み
	evilSpiritModel_ = (Model::CreateFromOBJ("enemy", true));
	// 悪霊のCSVファイル読み込み
	LoadEvilSpiritPopData();

	// 　良霊モデル読み込み
	goodSpiritModel_ = (Model::CreateFromOBJ("enemy_2", true));
	// 悪霊のCSVファイル読み込み
	LoadGoodSpiritPopData();

	// 　鏡モデル読み込み
	mirrorModel_ = (Model::CreateFromOBJ("mirror", true));
	// 鏡のCSVファイル読み込み
	LoadMirrorPopData();

	//// 境界の壁モデル読み込み
	//wallModel_ = (Model::CreateFromOBJ("block_WB", true));


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
	modelSkydome_ = Model::CreateFromOBJ("Sky", true);
	// 地面モデル初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_, {0.0f, -6.0f, 0.0f});

	// ガードレール

	// LoadGuardRailPopData();

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

	wall_ = std::make_unique<Wall>();
	// ステージ地面モデル読み込み
	wallModel_ = Model::CreateFromOBJ("block_WB", true);

	wall_->Initialize(wallModel_, {130.0f, 0.0f, 20.0f});

	wall_->GetWorldPosition();

	// summerSound_ = Audio::GetInstance()->LoadWave("Sound/summer.mp3"); // 晴BGM

	// Audio::GetInstance()->Audio::PlayWave(summerSound_, true, 0.5f);
	//   サウンド

	BGM_ = Audio::GetInstance()->LoadWave("Sound/BGM.mp3");
	cloudSound_ = Audio::GetInstance()->LoadWave("Sound/cloud.mp3");       // 雲
	moveSound_ = Audio::GetInstance()->LoadWave("Sound/button06.mp3");     // ADボタン
	decisionSound_ = Audio::GetInstance()->LoadWave("Sound/button01.mp3"); // 決定ボタン
	summerSound_ = Audio::GetInstance()->LoadWave("Sound/summer.mp3");     // 晴BGM
	gameOverSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameOver.mp3");
	gameClearSound_ = Audio::GetInstance()->Audio::LoadWave("Sound/gameClear.mp3");
	CarSound_ = Audio::GetInstance()->LoadWave("Sound/Car.mp3"); // 車走行

	Audio::GetInstance()->Audio::PauseWave(summerSound_);

	GravitySpeed = 5.0f;
}

void SunnyStage::Update() {

#pragma region 更新処理

	player_->Update();
	// Player の境界 (サイズ 0.25)
	Vector3 playerPos = player_->GetTranslate();
	Vector3 playerSpeed = player_->GetSpeed();

	if (start) {
		timer_->Update();
	}
	wall_->Update();
#pragma region CSV更新処理
	// 悪霊
	for (const std::unique_ptr<EvilSpirit>& evilSpirit_ : evilSpirits_) {
		evilSpirit_->Update();
	} // 良霊
	for (const std::unique_ptr<GoodSpirit>& goodSpirit_ : goodSpirits_) {
		goodSpirit_->Update();
	}
	// 鏡
	for (const std::unique_ptr<Mirror>& mirror_ : mirrors_) {
		mirror_->Update();
	}
	
	for (const std::unique_ptr<Box>& box_ : boxs_) {
		box_->Update();
	}
	// 加速装置
	for (const std::unique_ptr<Accelerator>& accelerator_ : accelerators_) {
		accelerator_->Update();
	}
	for (const std::unique_ptr<Box>& box_ : boxs_) {
		box_->Update();
	}

	for (const std::unique_ptr<InvisibleBox>& Blockbox_ : InvisiBoxs_) {
		Blockbox_->Update();
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
#pragma endregion
	skydome_->Update();

	if (timer_->GetStartTime() == 3) {
	}

	if (timer_->GetStartTime() <= 0 && start == false) {
		timer_->SetTimerFlag(true);
		// Audio::GetInstance()->Audio::PlayWave(CarSound_, true, 1.0f);
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

#pragma region ライトの当たり判定

	lightBackZ_ = player_->GetLightWorldPosition().z - 1.0f;
	lightFlontZ_ = player_->GetLightWorldPosition().z + 1.0f;
	lightLeftX_ = player_->GetLightWorldPosition().x - 1.0f;
	lightRightX_ = player_->GetLightWorldPosition().x + 1.0f;
	lightDownY_ = player_->GetLightWorldPosition().y - 1.0f;
	lightUpY_ = player_->GetLightWorldPosition().y + 1.0f;

#pragma endregion

#pragma region ライトと悪霊の当たり判定

	for (const std::unique_ptr<EvilSpirit>& evilSpirit_ : evilSpirits_) {

		bool boxMoveFlag = evilSpirit_->IsDead();

		EvilSpiritBackZ_ = evilSpirit_->GetWorldPosition().z - BackZHit_;
		EvilSpiritFlontZ_ = evilSpirit_->GetWorldPosition().z + FlontZHit_;
		EvilSpiritLeftX_ = evilSpirit_->GetWorldPosition().x - LeftXHit_;
		EvilSpiritRightX_ = evilSpirit_->GetWorldPosition().x + RightXHit_;
		EvilSpiritDownY_ = evilSpirit_->GetWorldPosition().y - DownHit_;
		EvilSpiritUpY_ = evilSpirit_->GetWorldPosition().y + UpHit_;

		if ((lightLeftX_ < EvilSpiritRightX_ && lightRightX_ > EvilSpiritLeftX_) &&
		    (EvilSpiritFlontZ_ > lightBackZ_ && EvilSpiritBackZ_ < lightFlontZ_) &&
		    (lightDownY_ < EvilSpiritUpY_ && lightUpY_ > EvilSpiritDownY_ && player_->GetLight())) {

			boxMoveFlag = true;

			if (boxMoveFlag) {
				Vector3 tmpTranslate = evilSpirit_->GetWorldPosition();

				tmpTranslate.y += 7.0f;

				if (timerFlag == false) {
					// player_->SetThunderHit(true);
					timerFlag = true;
				}

				evilSpirit_->SetTranslate(tmpTranslate);
				evilSpirit_->SetBoxFlag(boxMoveFlag);
				EnemyCount += 1;
			}
		}
	}

#pragma endregion

#pragma region プレイヤーと良霊の当たり判定

	for (const std::unique_ptr<GoodSpirit>& goodSpirit_ : goodSpirits_) {

		bool boxMoveFlag = goodSpirit_->IsDead();

		GoodSpiritBackZ_ = goodSpirit_->GetWorldPosition().z - BackZHit_;
		GoodSpiritFlontZ_ = goodSpirit_->GetWorldPosition().z + FlontZHit_;
		GoodSpiritLeftX_ = goodSpirit_->GetWorldPosition().x - LeftXHit_;
		GoodSpiritRightX_ = goodSpirit_->GetWorldPosition().x + RightXHit_;
		GoodSpiritDownY_ = goodSpirit_->GetWorldPosition().y - DownHit_;
		GoodSpiritUpY_ = goodSpirit_->GetWorldPosition().y + UpHit_;

		if ((PlayerLeftX_ < GoodSpiritRightX_ && PlayerRightX_ > GoodSpiritLeftX_) &&
		    (GoodSpiritFlontZ_ > PlayerBackZ_ && GoodSpiritBackZ_ < PlayerFlontZ_) &&
		    (PlayerDownY_ < GoodSpiritUpY_ && PlayerUpY_ > GoodSpiritDownY_)) {

			boxMoveFlag = true;

			if (input_->IsTriggerMouse(1) && boxMoveFlag) {
				Vector3 tmpTranslate = goodSpirit_->GetWorldPosition();

				tmpTranslate.y += 7.0f;

				if (timerFlag == false) {
					// player_->SetThunderHit(true);
					timerFlag = true;
				}

				goodSpirit_->SetTranslate(tmpTranslate);
				goodSpirit_->SetBoxFlag(boxMoveFlag);

				player_->SetLightCount(1);
			}
		}
	}

#pragma endregion

#pragma region プレイヤーとボックスの当たり判定

	bool isGrounded = false;

	// --- X方向の移動と判定 ---
	for (const std::unique_ptr<Box>& box : boxs_) {
		Vector3 boxPos = box->GetWorldPosition();

		float boxLeftX = boxPos.x - 1.0f;
		float boxRightX = boxPos.x + 1.0f;
		float boxTopY = boxPos.y + 1.5f;
		float boxBottomY = boxPos.y - 1.5f;

		float playerLeftX = playerPos.x - 1.0f;
		float playerRightX = playerPos.x + 1.0f;
		float playerTopY = playerPos.y + 1.5f;
		float playerBottomY = playerPos.y - 1.5f;

		// Y方向で重なっているときのみX判定する
		if (playerTopY > boxBottomY && playerBottomY < boxTopY) {
			if (playerRightX > boxLeftX && playerLeftX < boxRightX) {
				if (playerSpeed.x > 0) {
					/*	playerPos.x = boxLeftX - 1.5f;
					} else if (playerSpeed.x < 0) {
					    playerPos.x = boxRightX + 1.5f;
					}*/
					playerSpeed.x = 0.0f;
				}
			}
		}
	}

		// --- Y方向の移動と判定 ---
		for (const std::unique_ptr<Box>& box : boxs_) {
			Vector3 boxPos = box->GetWorldPosition();

			float boxLeftX = boxPos.x - 1.0f;
			float boxRightX = boxPos.x + 1.0f;
			float boxTopY = boxPos.y + 1.5f;
			float boxBottomY = boxPos.y - 1.5f;

			float playerLeftX = playerPos.x - 1.0f;
			float playerRightX = playerPos.x + 1.0f;
			float playerTopY = playerPos.y + 1.5f;
			float playerBottomY = playerPos.y - 1.5f;

			// X方向で重なっているときのみY判定する
			if (playerRightX > boxLeftX && playerLeftX < boxRightX) {
				if (playerSpeed.y > 0) {
					// 上に移動中 → 天井
					if (playerTopY > boxBottomY && playerBottomY < boxTopY) {
						playerPos.y = boxBottomY - 1.5f;
						playerSpeed.y = 0.0f;
					}
				} else if (playerSpeed.y < 0) {
					// 下に移動中 → 床
					if (playerTopY > boxBottomY && playerBottomY < boxTopY) {
						playerPos.y = boxTopY + 1.5f;
						playerSpeed.y = 0.0f;
						isGrounded = true;
					}
				}
			}
		}

		float MapMaxX = 61.0f;
	    float MapMinX = -61.0f;

		 if (playerPos.x > MapMaxX)
		    playerPos.x = MapMaxX;
	    if (playerPos.x < MapMinX)
		    playerPos.x = MapMinX;

		// --- 最終更新 ---
		player_->SetTranslate(playerPos);
		player_->SetSpeed(playerSpeed);

#pragma region プレイヤーと幽世ボックスの当たり判定

		playerPos.y -= GravitySpeed;

		float playerFeetY = playerPos.y; // プレイヤーの足元

		for (const std::unique_ptr<InvisibleBox>& Inbox : InvisiBoxs_) {
			Vector3 boxPos = Inbox->GetWorldPosition();
			float boxLeftX = boxPos.x - 0.5f;  // ブロックの左端
			float boxRightX = boxPos.x + 0.5f; // ブロックの右端
			float boxTopY = boxPos.y + 0.5f;   // ブロックの上面
			// float boxBottomY = boxPos.y - 0.5f;       // ブロックの底面

			// プレイヤーの幅（簡易）
			float playerLeftX = playerPos.x - 0.25f;
			float playerRightX = playerPos.x + 0.25f;

			// X方向でブロックに接触しているか
			bool hitX = (playerRightX > boxLeftX && playerLeftX < boxRightX);

			if (hitX) {
				// 上から落ちてきた場合、足場に乗せる
				if (playerFeetY >= boxTopY - 0.2f && playerFeetY <= boxTopY + 1.0f) {
					playerPos.y = boxTopY; // 足元をブロック上に固定
				}

				//// 左右にめり込んでいたら押し戻す
				// if (playerPos.x < boxLeftX)
				//  playerPos.x = boxLeftX - 0.5f;
				// if (playerPos.x > boxRightX)
				//  playerPos.x = boxRightX + 0.5f;
			}
		}

#pragma endregion

#pragma region プレイヤーとボックスの当たり判定
		// プレイヤーの移動速度（floatやVector2/3 などプロジェクトに合わせて定義済み想定）
		// playerSpeed = player_->GetSpeed();

		// for (const std::unique_ptr<Box>& box : boxs_) {
		//  Vector3 boxPos = box->GetWorldPosition();

		// // Box の境界
		// float boxLeftX = boxPos.x - 0.5f;
		// float boxRightX = boxPos.x;
		// float boxTopY = boxPos.y;
		// float boxBottomY = boxPos.y - 0.5f;

		// // Player の境界
		// float playerLeftX = playerPos.x - 0.25f;
		// float playerRightX = playerPos.x;
		// float playerTopY = playerPos.y;
		// float playerBottomY = playerPos.y - 0.25f;

		// // AABB判定
		// bool isColliding = (playerRightX > boxLeftX) && (playerLeftX < boxRightX) &&
		//                    (playerTopY > boxBottomY) && (playerBottomY < boxTopY);

		// if (isColliding) {
		//  // 横スクロールなので X 方向のスピードだけ止める
		//  //player_->GetSpeed().y = 0.0f;
		//  playerSpeed.x = 0.0f;
		//  playerSpeed.y = 0.0f;
		//  // Y方向の速度はそのまま（ジャンプや重力用）
		//  //break; // 1つでも当たったら止めるなら break
		// }

		//}

#pragma endregion

#pragma region プレイヤーと鏡の当たり判定

		for (const std::unique_ptr<Mirror>& mirror : mirrors_) {

			mirrorBackZ_ = mirror->GetWorldPosition().z - 1.0f;
			mirrorFlontZ_ = mirror->GetWorldPosition().z + 1.0f;
			mirrorLeftX_ = mirror->GetWorldPosition().x - 1.0f;
			mirrorRightX_ = mirror->GetWorldPosition().x + 1.0f;
			mirrorDownY_ = mirror->GetWorldPosition().y - 0.5f;
			mirrorUpY_ = mirror->GetWorldPosition().y + 1.5f;

			if ((PlayerLeftX_ < mirrorRightX_ && PlayerRightX_ > mirrorLeftX_) &&
			    (mirrorFlontZ_ > PlayerBackZ_ && mirrorBackZ_ < PlayerFlontZ_) &&
			    (PlayerDownY_ < mirrorUpY_ && PlayerUpY_ > mirrorDownY_)) {

				if (input_->IsTriggerMouse(1) && !mirrorFlag) {
					mirrorFlag = true;
					onInversion = true;
					followCamera_->SetInversion(onInversion);
					timer_->SetTimerFlag(true);
					mirrorCollider = true;
					player_->SetMirror(mirrorCollider);
					start = true;
					player_->SetStart(start);
				    player_->SetTranslate({
					    player_->GetWorldPosition().x,
						player_->GetWorldPosition().y,
				         player_->GetWorldPosition().z +3.0f
				    });
				} else if (input_->IsTriggerMouse(1) && mirrorFlag) {
					mirrorFlag = false;
					onInversion = false;
					followCamera_->SetInversion(onInversion);
					timer_->SetTimerFlag(false);
					mirrorCollider = false;
					player_->SetMirror(mirrorCollider);
					player_->SetTranslate(
					    {player_->GetWorldPosition().x, player_->GetWorldPosition().y,0});
				}
			}
			if ((PlayerLeftX_ > mirrorRightX_ && PlayerRightX_ < mirrorLeftX_) &&
			    (mirrorFlontZ_ < PlayerBackZ_ && mirrorBackZ_ > PlayerFlontZ_) &&
			    (PlayerDownY_ > mirrorUpY_ && PlayerUpY_ < mirrorDownY_)) {

				mirrorCollider = false;
				player_->SetMirror(mirrorCollider);
			}
		}

#ifdef _DEBUG
		if (input_->PushKey(DIK_LEFT)) {
			onInversion = true;
			followCamera_->SetInversion(onInversion);
			timer_->SetTimerFlag(true);
			player_->SetTranslate(
			    {player_->GetWorldPosition().x, player_->GetWorldPosition().y, 3});
		} else if (input_->PushKey(DIK_RIGHT)) {
			onInversion = false;
			followCamera_->SetInversion(onInversion);
			timer_->SetTimerFlag(false);
			player_->SetTranslate(
			    {player_->GetWorldPosition().x, player_->GetWorldPosition().y, 0});
		}
#endif
#pragma endregion

#pragma region プレイヤーと加速装置の当たり判定
		// 加速装置

		for (const std::unique_ptr<Accelerator>& accelerator_ : accelerators_) {
			SpeedBackZ_ = accelerator_->GetWorldPosition().z - 1.0f;
			SpeedFlontZ_ = accelerator_->GetWorldPosition().z + 1.0f;
			SpeedLeftX_ = accelerator_->GetWorldPosition().x - 1.0f;
			SpeedRightX_ = accelerator_->GetWorldPosition().x + 1.0f;

			if ((PlayerLeftX_ < SpeedRightX_ && PlayerRightX_ > SpeedLeftX_) &&
			    (SpeedFlontZ_ > PlayerBackZ_ && SpeedBackZ_ < PlayerFlontZ_)) {

				playerSpeed.x = 0.0f;
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
		evilSpirits_.remove_if([](std::unique_ptr<EvilSpirit>& item) {
			if (item->IsDead()) {
				item.release();
				return true;
			}
			return false;
		});

		// 悪霊のCSVファイルの更新処理
		UpdateEvilSpiritPopCommands();

		// デスフラグの立った敵を削除
		goodSpirits_.remove_if([](std::unique_ptr<GoodSpirit>& item) {
			if (item->IsDead()) {
				item.release();
				return true;
			}
			return false;
		});

		// 良霊のCSVファイルの更新処理
		UpdateGoodSpiritPopCommands();

		// デスフラグの立った敵を削除
		mirrors_.remove_if([](std::unique_ptr<Mirror>& item) {
			if (item->IsDead()) {
				item.release();
				return true;
			}
			return false;
		});

		// 鏡のCSVファイルの更新処理
		UpdateMirrorPopCommands();

		// デスフラグの立った敵を削除
		boxs_.remove_if([](std::unique_ptr<Box>& item) {
			if (item->IsDead()) {
				item.release();
				return true;
			}
			return false;
		});

		InvisiBoxs_.remove_if([](std::unique_ptr<InvisibleBox>& item) {
			if (item->IsDead()) {
				item.release();
				return true;
			}
			return false;
		});

		// ボックスのCSVファイルの更新処理
		UpdateBoxPopCommands();

		UpdateInvisibleBoxPopCommands();

		// デスフラグの立った敵を削除
		accelerators_.remove_if([](std::unique_ptr<Accelerator>& item) {
			if (item->IsDead()) {
				item.release();
				return true;
			}
			return false;
		});
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
		ImGui::InputFloat("PlayerDownSize_", &DownHit_, 0.1f);
		ImGui::InputFloat("PlayerUpSize_", &UpHit_, 0.1f);
		ImGui::End();

		ImGui::Begin("Clear ");
		ImGui::Checkbox("clearFlag", &goalTimerFlag);
		ImGui::InputInt("clearTimer", &EnemyCount);
		ImGui::End();

		ImGui::Begin("start timer ");
		ImGui::DragFloat("start timer pos x", &testPosTimer.x);
		ImGui::DragFloat("start timer pos y", &testPosTimer.y);
		ImGui::End();

#endif
	}

#pragma region タイム

	void SunnyStage::DrawTime() 
	{

		// ライトの数
		int eachMathNumber[2] = {};
		int mathNumber = player_->GetLightCount();
		int mathKeta = 10;

		for (int i = 0; i < 2; i++) {
			eachMathNumber[i] = mathNumber / mathKeta;
			mathNumber = mathNumber % mathKeta;
			mathKeta = mathKeta / 10;
		}
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
			// 残り時間描画
			spriteSecondTime_[i]->SetSize({64, 128});
			spriteSecondTime_[i]->SetTextureRect({32.0f * eachSecondNumber[i], 0}, {32, 64});
			spriteSecondTime_[i]->Draw();

			// 数描画
			spriteStartTime_[1]->SetSize({64, 128});
			spriteStartTime_[1]->SetPosition(testPosTimer);
			spriteStartTime_[1]->SetTextureRect({32.0f * eachMathNumber[1], 0}, {32, 64});
			spriteStartTime_[1]->Draw();
		}
	}
#pragma endregion

void SunnyStage::Draw() { 
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

	

	//for (const std::unique_ptr<Skydome>& startSkydome_ : startSkydomes_) {
	//	startSkydome_->Draw(viewProjection_);
	//}
	//for (const std::unique_ptr<Skydome>& MiddleSkydome_ : middleSkydomes_) {
	//	MiddleSkydome_->Draw(viewProjection_);
	//}
	//for (const std::unique_ptr<Skydome>& goalSkydome_ : goalSkydomes_) {
	//	goalSkydome_->Draw(viewProjection_);
	//}
	skydome_->Draw(viewProjection_);

	wall_->Draw(viewProjection_);

	for (const std::unique_ptr<Box>& box : boxs_) {
		box->Draw(viewProjection_);
	}

	for (const std::unique_ptr<InvisibleBox>& InBox : InvisiBoxs_)
	{
		InBox->Draw(viewProjection_);
	}


	/*for (const std::unique_ptr<EvilSpirit>& evilSpirit_ : evilSpirits_) {
		evilSpirit_->Draw(viewProjection_);
	}*/
	for (const std::unique_ptr<GoodSpirit>& goodSpirit_ : goodSpirits_) {
		goodSpirit_->Draw(viewProjection_);
	}
	for (const std::unique_ptr<Mirror>& mirror_ : mirrors_) {
		mirror_->Draw(viewProjection_);
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

#pragma region 悪霊 CSV

void SunnyStage::LoadEvilSpiritPopData() {
	evilSpiritPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/EvilSpiritPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	evilSpiritPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateEvilSpiritPopCommands() {
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

void SunnyStage::EvilSpiritGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	EvilSpirit* evilSpirit_ = new EvilSpirit();
	evilSpirit_->Initialize(evilSpiritModel_, position);
	evilSpirits_.push_back(static_cast<std::unique_ptr<EvilSpirit>>(evilSpirit_));
}
#pragma endregion

#pragma region 良霊 CSV

void SunnyStage::LoadGoodSpiritPopData() {
	goodSpiritPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/GoodSpiritPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	goodSpiritPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateGoodSpiritPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(goodSpiritPopCommands, line)) {
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

			GoodSpiritGenerate({x, y, z});
		}
	}
}

void SunnyStage::GoodSpiritGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	GoodSpirit* goodSpirit_ = new GoodSpirit();
	goodSpirit_->Initialize(goodSpiritModel_, position);
	goodSpirits_.push_back(static_cast<std::unique_ptr<GoodSpirit>>(goodSpirit_));
}
#pragma endregion

#pragma region 鏡 CSV

void SunnyStage::LoadMirrorPopData() {
	mirrorPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/MirrorPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	mirrorPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateMirrorPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(mirrorPopCommands, line)) {
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

			MirrorGenerate({x, y, z});
		}
	}
}

void SunnyStage::MirrorGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	Mirror* mirror_ = new Mirror();
	mirror_->Initialize(mirrorModel_, position);
	mirrors_.push_back(static_cast<std::unique_ptr<Mirror>>(mirror_));
}
#pragma endregion

#pragma region ボックス CSV

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

#pragma region 幽世ボックス

void SunnyStage::LoadInvisibleBoxPopData()
{
	InvisiBoxPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/InvisibleBoxPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	InvisiBoxPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void SunnyStage::UpdateInvisibleBoxPopCommands() 
{
	std::string line;

	// コマンド実行ループ
	while (getline(InvisiBoxPopCommands, line)) {
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

			InvisibleBoxGenerate({x, y, z});
		}
	}
}

void SunnyStage::InvisibleBoxGenerate(Vector3 position)
{
	// アイテムの生成と初期化処理
	InvisibleBox* InBox_ = new InvisibleBox();
	InBox_->Initialize(BlockModel_, position);
	InvisiBoxs_.push_back(static_cast<std::unique_ptr<InvisibleBox>>(InBox_));
}





#pragma endregion

#pragma region 敵 CSV

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

void SunnyStage::StartSkydomeGenerate(Vector3 position) { // アイテムの生成と初期化処理
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

void SunnyStage::LoadGoalSkydomePopData() 
{

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
	InvisiBoxs_.clear();
	evilSpirits_.clear();
	mirrors_.clear();
	goodSpirits_.clear();
	accelerators_.clear();
	startSkydomes_.clear();
	middleSkydomes_.clear();
	goalSkydomes_.clear();

	timer_->SetTime(0, 30);
	timer_->SetTimerFlag(false);
	followCamera_->SetPos({0, 4, 0});

	sceneNo = SELECT;
}

void SunnyStage::Goal() {

	if (EnemyCount > 12)
	{
		EnemyCount = 0;
		player_->SetLightCount(0);
		mirrors_.clear();
		sceneNo = CLEAR;

	}

	if (timer_->GetTimeSecond() <= 0 ) {
		boxs_.clear();
		evilSpirits_.clear();
		mirrors_.clear();
		goodSpirits_.clear();
		accelerators_.clear();
		startSkydomes_.clear();
		middleSkydomes_.clear();
		goalSkydomes_.clear();
		trafficLight_.clear();
		goalTimer = 0;
		goalTimerFlag = false;
		start = false;
		EnemyCount = 0;
		//mirrorFlag = false;

		// Audio::GetInstance()->Audio::StopWave(summerSound_);
		// Audio::GetInstance()->Audio::StopWave(CarSound_);
		player_->SetLightCount(0);
		timer_->SetTime(0, 30);
		timer_->SetTimerFlag(false);
		followCamera_->SetPos({0, 4, 0});

		sceneNo = END;
	}
	if (player_->GetLightCount() <= 0 && start) {
		boxs_.clear();
		evilSpirits_.clear();
		mirrors_.clear();
		goodSpirits_.clear();
		accelerators_.clear();
		startSkydomes_.clear();
		middleSkydomes_.clear();
		goalSkydomes_.clear();
		trafficLight_.clear();
		goalTimer = 0;
		goalTimerFlag = false;
		start = false;
		EnemyCount = 0;
		// mirrorFlag = false;

		// Audio::GetInstance()->Audio::StopWave(summerSound_);
		// Audio::GetInstance()->Audio::StopWave(CarSound_);
		player_->SetLightCount(0);
		timer_->SetTime(0, 30);
		timer_->SetTimerFlag(false);
		followCamera_->SetPos({0, 4, 0});

		sceneNo = END;
	}
}
