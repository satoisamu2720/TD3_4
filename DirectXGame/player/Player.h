#pragma once
#define _USE_MATH_DEFINES
#include "ImGuiManager.h"
#include "Model.h"
#include "Input.h"
#include "math.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <cassert>
#include <list>
#include <stdio.h>
#include <optional>
#include "baseCharacter/BaseCharacter.h"
#include "VectraCalculation.h"

class Player : public BaseCharacter {

private:
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformBack_;
	WorldTransform worldTransformFront_;

	/*WorldTransform worldTransformFrontLeft_;
	WorldTransform worldTransformFrontRight_;
	WorldTransform worldTransformBackLeft_;
	WorldTransform worldTransformBackRight_;*/

	Vector3 position = {0, -2.5f, 8.0f};
	Vector3 bodyPosition = {0, 0, 0};
	Vector3 frontPosition = {0, 0, 0.5f};
	Vector3 backPosition = {0, 0, -0.5f};

	/*Vector3 frontLeftPosition = {0, 0, 0};
	Vector3 frontRightPosition = {0, 0, 0};
	Vector3 backLeftPosition = {0, 0, 0};
	Vector3 backRightPosition = {0.0f, 0.0f, 0.0f};*/

public:
	//プレイヤー初期化
	void Initialize(const std::vector<Model*>& models);

	//更新処理
	void Update();

	//プレイヤーの角度制限
	void RightMove();
	void LeftMove();

	//晴時の更新処理
	void SunnyUpdate();

	void NotHitInitialize();

	// 障害物に当たった時の初期化
	void NormalHitMotionInitialize();
	//障害物に当たった時の更新処理
	void NormalHitMotion();

	//雷雨時の更新処理
	void ThunderstormUpdate();
	//雷に当たった時の初期化
	void ThunderHitMotionInitialize();
	// 雷に当たった時の更新処理
	void ThunderHitMotion();


	//プレイヤー描画
	void Draw(ViewProjection& view);
	
	Vector3 GetWorldPosition();


	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	

	void SetTranslate(Vector3 translate) { worldTransform_.translation_ = translate; };

	 float GetWeather() { return weather_; }
	 void  SetWeather(float weather) { weather_ = weather; }

	//ノーマル
	void SetNormalHit(bool normalHit) { normalHit_ = normalHit; } 
	//bool GetNormalHit() { return normalHit_; } 

	//雷雨
	void SetRand(float setRand) { setRand_ = setRand; }
	void SetThunderHit(bool thunderHit) { thunderHit_ = thunderHit; }

	void SetPosition(float position_) { position.z = position_; }

	~Player();

	enum class WeatherHit {
		NotHit,
		Normal,      // 通常
		Thunder,    // 雷
		
	};

private:
	Vector3 move_ = {0, 0, 0};

	bool notRotate = false;
	// キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;

	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	 float weather_ = 0;
	//ノーマル当たり判定

	bool normalHit_ = false;
	float normalHitRootParameter_ = 0.0f;
	float normalHitTime = 0;
	float bestRotation = 12.0f;

	//雷雨
	bool thunderHit_ = false;
	float wind = 0.0f;
	float windLeft =  0.2f;
	float windRight = -0.2f;
	float setRand_ = 1;
	float thunderHitRootParameter_ = 0.0f;


		WeatherHit weatherHit_ = WeatherHit::NotHit;
	std::optional<WeatherHit> weatherHitRequest_ = std::nullopt;
};

