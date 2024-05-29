#pragma once
#include "Audio.h"
#include "AxisIndicator.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "MT.h"
#include "Model.h"
#include "Obstacle/accelerator/Accelerator.h"
#include "Obstacle/box/Box.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "camera/railCamera/RailCamera.h"
#include "player/Player.h"
#include "scene/IScene.h"
#include "stage/ground/Ground.h"
#include "stage/guardRail/GuardRail.h"
#include "stage/skydome/Skydome.h"
#include "timer.h"
#include <cassert>
#include <fstream>
#include <memory>

#include "CircleShadow.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "math.h"

class  FogStage  : public IScene {

public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Time();

	// タイム
	void DrawTime();

	void Reset();

	void Goal();

#pragma region ボックスCSV関数

	// ボックス発生データを読み込み
	void LoadBoxPopData();

	// ボックスの発生コマンドの更新
	void UpdateBoxPopCommands();

	/// ボックスの生成
	void BoxGenerate(Vector3 position);

#pragma endregion

#pragma region 加速装置CSV関数

	// 加速装置の発生データを読み込み
	void LoadAcceleratorPopData();

	// 加速装置の発生コマンドの更新
	void UpdateAcceleratorPopCommands();

	/// 加速装置の生成
	void AcceleratorGenerate(Vector3 position);

#pragma endregion

#pragma region 背景CSV関数

	// 開始背景の発生データを読み込み
	void LoadStartSkydomePopData();

	// 開始背景の発生コマンドの更新
	void UpdateStartSkydomePopCommands();

	/// 開始背景の生成
	void StartSkydomeGenerate(Vector3 position);

	// 直線背景の発生データを読み込み
	void LoadMiddleSkydomePopData();

	// 直線背景の発生コマンドの更新
	void UpdateMiddleSkydomePopCommands();

	/// 直線背景の生成
	void MiddleSkydomeGenerate(Vector3 position);

	// ゴール背景の発生データを読み込み
	void LoadGoalSkydomePopData();

	// ゴール背景の発生コマンドの更新
	void UpdateGoalSkydomePopCommands();

	/// ゴール背景の生成
	void GoalSkydomeGenerate(Vector3 position);

	// 信号機の発生データを読み込み
	void LoadGuardRailPopData();

	// 信号機の発生コマンドの更新
	void UpdateGuardRailPopCommands();

	/// 信号機の生成
	void trafficLight(Vector3 position);
#pragma endregion

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// ライト
	LightGroup* light_ = nullptr;

	std::unique_ptr<Timer> timer_;

	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> modelPlayerBody_;
	std::unique_ptr<Model> modelPlayerFront_;
	std::unique_ptr<Model> modelPlayerBack_;
	std::list<Player*> players_;

	// 障害物

	// ボックス
	std::list<std::unique_ptr<Box>> boxs_;
	// ボックスの発生コマンド
	std::stringstream boxPopCommands;
	// モデル
	Model* BoxModel_ = nullptr;

	// 加速装置
	std::list<std::unique_ptr<Accelerator>> accelerators_;
	// 加速装置の発生コマンド
	std::stringstream acceleratorPopCommands;
	// モデル
	Model* acceleratorModel_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;

	uint32_t texHandle_ = 0;
	float inputFloat3[3] = {0, 0, 0};

	Vector3 color = {1.0f, 1.0f, 1.0f};

	// カメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<RailCamera> railCamera_;

	// スタートステージ
	std::list<std::unique_ptr<Skydome>> startSkydomes_;
	// ステージの発生コマンド
	std::stringstream startSkydomePopCommands;

	// 直線ステージ
	std::list<std::unique_ptr<Skydome>> middleSkydomes_;
	// ステージの発生コマンド
	std::stringstream middleSkydomePopCommands;

	// ゴールステージ
	std::list<std::unique_ptr<Skydome>> goalSkydomes_;
	// ステージの発生コマンド
	std::stringstream goalSkydomePopCommands;

	// 信号機
	std::list<std::unique_ptr<TrafficLight>> trafficLight_;
	// 発生コマンド
	std::stringstream TrafficLightPopCommands;

	Model* modelSkydome_ = nullptr;
	Model* modelStartSkydome_ = nullptr;
	Model* modelMiddleSkydome_ = nullptr;
	Model* modelGoalSkydome_ = nullptr;

	Model* modelTrafficLight_ = nullptr;

	std::unique_ptr<Ground> ground_;
	Model* modelGround_ = nullptr;

	Vector3 velocity_;
	bool isDebugcameraActive_ = false;

	// 確認用あたり判定

	float FlontZHit_ = 2.0f;
	float BackZHit_ = 2.0f;
	float RightXHit_ = 2.0f;
	float LeftXHit_ = 2.0f;

	// プレイヤーの当たり判定
	float PlayerFlontZ_;
	float PlayerBackZ_;
	float PlayerRightX_;
	float PlayerLeftX_;

	// ボックスの当たり判定
	float BoxFlontZ_;
	float BoxBackZ_;
	float BoxRightX_;
	float BoxLeftX_;

	// 加速装置の当たり判定
	float SpeedFlontZ_;
	float SpeedBackZ_;
	float SpeedRightX_;
	float SpeedLeftX_;

	// ゴールの当たり判定
	float goalFlontZ_;
	float goalBackZ_;
	float goalRightX_;
	float goalLeftX_;

	// 天候
	float weather = 0;

	// ゲームスタート
	bool start;

	bool timerFlag = false;
	float timer = 0;

	uint32_t buttonSound_;

	uint32_t titleTexHandle_ = 0;
	Sprite* titleSprite_ = nullptr;

	Vector3 csv = {0, 0, 0};

	// ナンバー
	uint32_t textureHandleNumber_ = 0;
	Sprite* spriteMathTime_[2] = {};
	Sprite* spriteSecondTime_[2] = {};
	int gameScore_ = 10;

	// クリアフラグ
	bool clearTimerFlag = false;
	float clearTimer = 0;

	// ゴールフラグ
	bool goalTimerFlag = false;
	float goalTimer = 0;

	//霧の画像
	uint32_t fogTexture_;
	uint32_t BlendTexture_;
	Sprite* fogsprite_;
	Sprite* BlendSprite_;

	Vector2 pos;

	WorldTransform Pos_ = {};

	Vector2 scale = {};
	Vector4 fogColor = {};
	Vector4 fogspeed{};

	WorldTransform ColorTransform_ = {};
};

