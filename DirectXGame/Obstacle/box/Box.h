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
#include "VectraCalculation.h"
class Box  {
private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	// デスフラグ
	bool isDead_ = false;

public:
	// プレイヤー初期化
	void Initialize(Model* model,Vector3 position);

	// 更新処理
	void Update();

	//描画
	void Draw(ViewProjection& view);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();
};
