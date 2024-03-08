﻿#pragma once

#include "ImGuiManager.h"
#include "VectraCalculation.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "input.h"
#include <memory>

class RailCamera {

public:
	void Initialize(const Vector3& position, const Vector3& rotation);
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }
	void SetParent(const WorldTransform* parent) {
		worldTransform_.parent_ = parent;
	}
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }


private:

	const WorldTransform* target_ = nullptr;
	Input* input_ = nullptr;

	// キャラクターの移動速度
	const float kCharacterSpeed = 3.0f;
	const float cameraSpeed = 0.05f;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};

