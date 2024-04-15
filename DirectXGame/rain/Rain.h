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

class Rain : public BaseCharacter {

  public: 
	void Initalize(const std::vector<Model*>& models);

	void Update();

	

	void Draw(ViewProjection& viewProjection);

	void SetTarget(const WorldTransform* target) { target_ = target; }


  private:
	WorldTransform worldTransform_[3];

	WorldTransform fastTransform_[3];

	Model* model_ = nullptr;

	// 3Dモデル
	Model* aitem = nullptr;

	Input* input_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// 　カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	const WorldTransform* target_ = nullptr;


};
