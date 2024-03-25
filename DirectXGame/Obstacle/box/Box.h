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
class GameScene;

class Box  {
private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	
	GameScene* gameScene_ = nullptr;

public:
	// プレイヤー初期化
	void Initialize(Model* model,Vector3 position);

	// 更新処理
	void Update();

	//描画
	void Draw(ViewProjection& view);

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	Vector3 GetWorldPosition();
};
