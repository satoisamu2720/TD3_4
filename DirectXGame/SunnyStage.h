﻿#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Iscene.h"
#include "Model.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "player/Player.h"
#include "camera/railCamera/RailCamera.h"
#include "stage/skydome/Skydome.h"
#include "stage/ground/Ground.h"
#include "Obstacle/box/Box.h"
#include "Obstacle/accelerator/Accelerator.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "VectraCalculation.h"
#include <memory>
#include <fstream>


class SunnyStage : public IScene {

public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Time();

	/// <summary>
	/// ボックス発生データを読み込み
	/// </summary>
	void LoadBoxPopData();

	/// <summary>
	/// ボックスの発生コマンドの更新
	/// </summary>
	void UpdataBoxPopCommands();

	/// <summary>
	/// ボックスの生成
	/// </summary>
	/// <param name="positioin"></param>
	void BoxGenerate(Vector3 position);

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> modelPlayerBody_;
	std::unique_ptr<Model> modelPlayerFront_;
	std::unique_ptr<Model> modelPlayerBack_;
	std::list<Player*> players_;

	// 障害物
	std::unique_ptr<Box> box_;
	Model* BoxModel_ = nullptr;

	// 加速装置
	std::unique_ptr<Accelerator> accelerator_[2];
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

	// ステージ
	std::unique_ptr<Skydome> skydome_[2];
	Model* modelSkydome_ = nullptr;

	std::unique_ptr<Ground> ground_;
	Model* modelGround_ = nullptr;

	Vector3 velocity_;
	bool isDebugcameraActive_ = false;

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

	//天候
	float weather = 0;

	//ゲームスタート
	bool start = false;

	bool timerFlag = false;
	float timer = 0;

	uint32_t buttonSound_;

	uint32_t titleTexHandle_ = 0;
	Sprite* titleSprite_ = nullptr;

	// ボックス
	std::list<std::unique_ptr<Box>> boxs_;
	// ボックスの発生コマンド
	std::stringstream boxPopCommands;


};
