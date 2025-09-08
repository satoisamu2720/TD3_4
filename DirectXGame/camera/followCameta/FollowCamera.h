#pragma once
#include "ImGuiManager.h"
#include "Input.h"
#include "MT.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>

class FollowCamera {
public:
	void Initialize(const Vector3& position, const Vector3& rotation);

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }
	void SetPos(const Vector3 pos) { worldTransform_.translation_ = pos; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	Matrix4x4 GetRotateYMatrix_() { return rotateYMatrix_; }

	void PlayerSetTransformPosition(Vector3 Position) { playerSetTransformPosition = Position; }
	void PlayerSetTransformRotation(Vector3 Rotation) { playerSetTransformRotation = Rotation; }

private:
	const WorldTransform* target_ = nullptr;
	ViewProjection viewProjection_;
	WorldTransform worldTransform_;
	Vector3 playerSetTransformRotation;
	Vector3 playerSetTransformPosition;
	Input* input_ = nullptr;
	Matrix4x4 rotateYMatrix_;
};