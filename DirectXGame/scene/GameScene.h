#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "AxisIndicator.h"
#include "player/Player.h"
#include "MT/MT.h"
#include "camera/railCamera/RailCamera.h"
#include "camera/followCamera/FollowCamera.h"
#include "skydome/Skydome.h"
#include "spirit/evilSpirit/EvilSpirit.h"

#include <memory>
#include <fstream>
#include <cassert>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	#pragma region ボックスCSV関数

	// ボックス発生データを読み込み
	void LoadEvilSpiritPopData();

	// ボックスの発生コマンドの更新
	void UpdateEvilSpiritPopCommands();

	/// ボックスの生成
	void EvilSpiritGenerate(Vector3 position);

#pragma endregion

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	bool isDebugcameraActive_ = false;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> modelPlayer_;
	std::list<Player*> players_;

	std::unique_ptr<Skydome> skydome_;
	Model* modelSkydome_ = nullptr;

	//悪霊
	std::list<std::unique_ptr<EvilSpirit>> evilSpirits_;
	// 悪霊の発生コマンド
	std::stringstream evilSpiritPopCommands;
	Model* evilSpiritModel_ = nullptr;

	// カメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<RailCamera> railCamera_;
	std::unique_ptr<FollowCamera> followCamera_;

	// プレイヤーの当たり判定
	float PlayerFlontZ_;
	float PlayerBackZ_;
	float PlayerRightX_;
	float PlayerLeftX_;

	// ボックスの当たり判定
	float EvilSpiritFlontZ_;
	float EvilSpiritBackZ_;
	float EvilSpiritRightX_;
	float EvilSpiritLeftX_;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
