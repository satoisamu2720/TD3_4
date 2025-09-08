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
#include "MT.h"

class Player : public BaseCharacter {

private:
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformLight_;

	Vector3 position = {-4, 1, 0.0f};
	Vector3 bodyPosition = {0, 0, 0};
	Vector3 lightPosition_ = {2, 0, 0}; 

public:
	//プレイヤー初期化
	void Initialize(const std::vector<Model*>& models);

	//更新処理
	void Update();

	

	//プレイヤー描画
	void Draw(ViewProjection& view);
	
	Vector3 GetWorldPosition();

	Vector3 GetLightWorldPosition();


	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	

	void SetTranslate(Vector3 translate) { worldTransform_.translation_ = translate; };
	Vector3 GetTranslate() {return worldTransform_.translation_; };

	 bool GetLight() { return lightFlag; }
	 void  SetWeather(float weather) { weather_ = weather; }


	void SetPosition(float position_) { position.z = position_; }

	void SetStart(bool start) { gameStart = start; }

	const WorldTransform* GetWorldTransformPtr() const { return &worldTransform_; }

	~Player();


private:
	Vector3 move_ = {0, 0, 0};
	Vector3 rotMove_ = {0, 0, 0};

	bool gameStart = false;
	bool lightFlag = false;
	int lightCount = 1;
	float lightTimer = 0;
	// キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;

	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	 float weather_ = 0;


};

