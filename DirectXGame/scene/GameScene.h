#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "player/Player.h"
#include "camera/railCamera/RailCamera.h"
#include "stage/skydome/Skydome.h"
#include "stage/ground/Ground.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>
#include "timer/Timer.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: 
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
private: 
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;


	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> modelPlayerBody_;
	std::unique_ptr<Model> modelPlayerFrontLeft_;
	std::unique_ptr<Model> modelPlayerFrontRight_;
	std::unique_ptr<Model> modelPlayerBackLeft_;
	std::unique_ptr<Model> modelPlayerBackRight_;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;

	float inputFloat3[3] = {0, 0, 0};

	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<RailCamera> railCamera_;

	std::unique_ptr<Timer> timer_;

	std::unique_ptr<Skydome> skydome_;
	Model* modelSkydome_ = nullptr;

	std::unique_ptr<Ground> ground_;
	Model* modelGround_ = nullptr;
	
	Vector3 velocity_;
	bool isDebugcameraActive_ = false;

	bool non = false;
};