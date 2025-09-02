#pragma once
#define _USE_MATH_DEFINES
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
#include "baseCharacter/BaseCharacter.h"
#include "mt/MT.h"

class Player : public BaseCharacter {
private:
	WorldTransform worldTransform_;
	Vector3 position = {0.0f, 0.0f, 0.0f};
	Vector3 move_ = {0, 0, 0};
	// キャラクターの移動速度
	const float Speed = 0.2f;

	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

public:
	// プレイヤー初期化
	void Initialize(const std::vector<Model*>& models);
	// 更新処理
	void Update();

	// プレイヤー描画
	void Draw(ViewProjection& view);

	Vector3 GetWorldPosition();

	~Player();

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
};
