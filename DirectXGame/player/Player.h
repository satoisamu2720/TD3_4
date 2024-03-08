﻿#pragma once
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
	WorldTransform worldTransformFront_;
	WorldTransform worldTransformL_;
	WorldTransform worldTransformR_;
	WorldTransform worldTransformBack_;

	Vector3 position = {0, -8.0f, 75.0f};
	Vector3 bodyPosition = {0, 0, 0};
	Vector3 frontPosition = {0, 0, 1};
	Vector3 lPosition = {0.5f, 0, 0};
	Vector3 rPosition = {-0.5f, 0, 0};
	Vector3 backPosition = {0.0f, 0.0f, -0.65f};

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

