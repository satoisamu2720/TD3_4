#pragma once
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
#include "MT/MT.h"
class EvilSpirit {
private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	// デスフラグ
	bool isDead_ = false;

public:
	// プレイヤー初期化
	void Initialize(Model* model, Vector3 position);

	// 更新処理
	void Update();

	// 描画
	void Draw(ViewProjection& view);

	bool IsDead() const { return isDead_; }

	void SetTranslate(Vector3 translate);

	void SetBoxFlag(bool boxflag) { isDead_ = boxflag; }

	Vector3 GetWorldPosition();

	const WorldTransform* GetWorldTransformPtr() const { return &worldTransform_; }
};
