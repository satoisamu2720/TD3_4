#pragma once
#define _USE_MATH_DEFINES
#include "ImGuiManager.h"
#include "Input.h"
#include "math.h"
#include "Model.h"
#include "WorldTransform.h"
#include "baseCharacter/BaseCharacter.h"
#include "math/VectraCalculation.h"
#include <list>
#include <stdio.h>
#include <cassert>
class Player : public BaseCharacter {

private:

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformFront_;
	WorldTransform worldTransformL_;
	WorldTransform worldTransformR_;
	WorldTransform worldTransformBack_;

	Vector3 velocity_ = {0, 0, 10};
	Vector3 position = {0, 1, 0};
	Vector3 bodyPosition = {0, 0, 0};
	Vector3 frontPosition = {0, 0, 1};
	Vector3 lPosition = {0.5f, 0, 0};
	Vector3 rPosition = {-0.5f, 0, 0};
	Vector3 backPosition = {0.0f, 0.0f, -0.65f};

public:
	void Initialize(const std::vector<Model*>& models);
	void Update();
	void Draw(ViewProjection view);

	Vector3 GetWorldPosition();

	/*void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}*/
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	~Player();

private:
	WorldTransform worldTransform_;
	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;
};
