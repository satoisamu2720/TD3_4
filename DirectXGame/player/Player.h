#pragma once
#define _USE_MATH_DEFINES
#include "ImGuiManager.h"
#include "Model.h"
#include "Input.h"
#include "math.h"
#include "WorldTransform.h"
#include <cassert>
#include <list>
#include <stdio.h>
#include "baseCharacter/BaseCharacter.h"
#include "VectraCalculation.h"
#include "ViewProjection.h"

class Player : public BaseCharacter {

private:
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformFrontLeft_;
	WorldTransform worldTransformFrontRight_;
	WorldTransform worldTransformBackLeft_;
	WorldTransform worldTransformBackRight_;

	Vector3 position = {0, -8.0f, 75.0f};
	Vector3 bodyPosition = {0, 0, 0};
	Vector3 frontLeftPosition = {0, 0, 0};
	Vector3 frontRightPosition = {0, 0, 0};
	Vector3 backLeftPosition = {0, 0, 0};
	Vector3 backRightPosition = {0.0f, 0.0f, 0.0f};

public:
	//プレイヤー初期化
	void Initialize(const std::vector<Model*>& models);

	//プレイヤーの角度制限
	void RightMove();
	void LeftMove();

	//晴時の更新処理
	void SunnyUpdate();
	void NormalHitMotion();

	//雷雨時の更新処理
	void ThunderstormUpdate();
	// 雷に当たった時の更新処理
	void ThunderHitMotion();

	//
	

	//プレイヤー描画
	void Draw(ViewProjection& view);

	Vector3 GetWorldPosition();

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }


	//ノーマル
	void SetNormalHit(bool normalHit) { normalHit_ = normalHit; } 

	//雷雨
	void SetRand(float setRand) { setRand_ = setRand; }
	void SetThunderHit(bool thunderHit) { thunderHit_ = thunderHit; }


	~Player();

private:
	Vector3 move_ = {0, 0, 0};

	// キャラクターの移動速度
	const float kCharacterSpeed = 1.0f;

	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	//ノーマル当たり判定
	bool normalHit_ = false;
	float normalHitRootParameter_ = 0.0f;
	float normalHitTime = 140;

	//雷雨
	bool thunderHit_ = false;
	float wind = 0.0f;
	float windLeft =  0.2f;
	float windRight = -0.2f;
	float setRand_ = 1;
	float thunderHitRootParameter_ = 0.0f;
};

