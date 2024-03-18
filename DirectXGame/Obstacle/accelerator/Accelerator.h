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

	//Vector3 position = {20.0f, -8.0f, 200.0f};
	

public:
	// プレイヤー初期化
	void Initialize(Model* model, Vector3 position);

	// 更新処理
	void Update();

	// 描画
	void Draw(ViewProjection& view);

	Vector3 GetWorldPosition();
};
