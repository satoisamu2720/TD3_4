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
class Accelerator {
private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Vector3 position = {0, 0.0f, 60.0f};

public:
	// プレイヤー初期化
	void Initialize(Model* model);

	// 更新処理
	void Update();

	// 描画
	void Draw(ViewProjection& view);

	Vector3 GetWorldPosition();
};
