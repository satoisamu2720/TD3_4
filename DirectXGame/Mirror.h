#pragma once

#include "ImGuiManager.h"
#include "Input.h"
#include "MT.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "math.h"
#include <cassert>
#include <list>
#include <optional>
#include <stdio.h>
class Mirror {
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
};
