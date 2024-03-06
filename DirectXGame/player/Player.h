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
	WorldTransform worldTransformFront_;
	WorldTransform worldTransformL_;
	WorldTransform worldTransformR_;
	WorldTransform worldTransformBack_;

	Vector3 position = {0, 1, 0};
	Vector3 bodyPosition = {0, 0, 0};
	Vector3 frontPosition = {0, 0, 1};
	Vector3 lPosition = {0.5f, 0, 0};
	Vector3 rPosition = {-0.5f, 0, 0};
	Vector3 backPosition = {0.0f, 0.0f, -0.65f};

public:
	void Initialize(const std::vector<Model*>& models);
	void Update();
	void Draw(ViewProjection& view);
	void SetTranslate(Vector3 translate);

	Vector3 GetWorldPosition();

	//Vector3 GetWorldTransform(){return worldTransform_.translation_;};

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	
	Vector3 GetWorldTransformBack() { return worldTransformBack_.translation_; };
	Vector3 GetWorldTransformfront() { return worldTransformFront_.translation_; };
	Vector3 GetWorldTransformR() { return worldTransformR_.translation_; };
	Vector3 GetWorldTransformL() { return worldTransformL_.translation_; };



	~Player();

private:
	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;
};

