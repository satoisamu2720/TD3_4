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
	void Initialize(const std::vector<Model*>& models);
	void Update();
	void Draw(ViewProjection& view);
	void RightMove();
	void LeftMove();

	Vector3 GetWorldPosition();

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	~Player();

private:
	Vector3 move_ = {0, 0, 0};

	// キャラクターの移動速度
	const float kCharacterSpeed = 1.0f;

	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;
};

