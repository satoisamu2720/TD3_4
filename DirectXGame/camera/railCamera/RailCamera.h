#pragma once

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

	//加速装置
	bool GetIsSpeedUp() { return isSpeedUp; }
	bool GetIsSpeedDown() { return isSpeedDown; }

	void SetIsSpeedUp(bool isSpeedUp_) { isSpeedUp =  isSpeedUp_; }
	void SetIsSpeedDown(bool isSpeedDown_) { isSpeedDown = isSpeedDown_; }

private:

	const WorldTransform* target_ = nullptr;
	Input* input_ = nullptr;

	// キャラクターの移動速度
	const float kCharacterSpeed = 3.0f;
	const float cameraSpeed = 0.05f;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;


	bool isSpeedUp = false;
	bool isSpeedDown = false;

	float isSpeedTime = 0.0f;

};

