#pragma once
#include <cassert>
#include "Model.h"
#include "WorldTransform.h"
class Cloud {
public:
	void Initialize(Model* model,bool flag);

	void Update();

	void Draw(ViewProjection& view);

	bool IsDead() const { return isDead_; }

	bool GetMoveFlag() { return moveFlag; }
	void SetMoveFlag(bool flag) {  moveFlag = flag; }

	Vector3 GetWorldPosition();

public:
	WorldTransform worldTransform_[4];
	Model* model_ = nullptr;
	float moveSpeed = 0.5;
	// デスフラグ
	bool isDead_ = false;

	bool selectFlag = false;

	bool moveFlag = false;

};
