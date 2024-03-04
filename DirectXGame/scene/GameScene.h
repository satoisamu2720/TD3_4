#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
//#include "Ground.h"
#include "Input.h"
#include "Model.h"
#include "player/Player.h"
//#include "RailCamera.h"
//#include "SafeDelete.h"
//#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>
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

	std::unique_ptr<Model> modelPlayerBody_;
	std::unique_ptr<Model> modelPlayerFront_;
	std::unique_ptr<Model> modelPlayerL_;
	std::unique_ptr<Model> modelPlayerR_;
	std::unique_ptr<Model> modelPlayerBack_;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;

	float inputFloat3[3] = {0, 0, 0};

	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<Player> player_;
	//std::unique_ptr<Skydome> skydome_;
	//Model* modelSkydome_ = nullptr;
	//std::unique_ptr<Ground> ground_;
	//Model* modelGround_ = nullptr;
	//std::unique_ptr<RailCamera> railCamera_;

	Vector3 velocity_;
	bool isDebugcameraActive_ = false;
	
};