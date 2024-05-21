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
#include "ViewProjection.h"

class Rain  {

  public: 
	void Initialize(Model* model);

	void Update();

	void SetWindFlag(bool windFlag) { windFlag_ = windFlag; }

	void Draw(ViewProjection& viewProjection);

	void SetTarget(const WorldTransform* target) { target_ = target; }


  private:
	WorldTransform worldTransform_[15];

	WorldTransform fastTransform_[15];

	// 3Dモデル
	Model* rainModel[15] = {nullptr};

	Model* model_ = nullptr;

	bool windFlag_ = false;

	Input* input_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// 　カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	const WorldTransform* target_ = nullptr;


};
