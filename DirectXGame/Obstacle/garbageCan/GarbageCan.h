#pragma once
#define _USE_MATH_DEFINES
#include <cassert>
#include <optional>
#include "Model.h"
#include "WorldTransform.h"
#include "math.h"
#include "Input.h"
#include "MT.h"
class GarbageCan {
public:
	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(ViewProjection& view);

	void SetTranslate(Vector3 translate);


    void SetRotate(bool SetRotate) { rotate = SetRotate; }

	bool GetRotate() { return rotate; }

	bool IsDead() const { return isDead_; }

	void SetGarbageCanFlag(bool GarbageCanFlag) { isDead_ = GarbageCanFlag; }

	void NotHitMotionInitialize();
	void NotHitMotion();

	void YesHitMotionInitialize();
	void YesHitMotion();

	Vector3 GetWorldPosition();

	enum class Hit {
		NotHit,
		YesHit,
	};


public:

	bool rotate = false;
	float NotRootParameter_ = 0.0f;
	float NotTime = 60;
	float NotBestRotation = 12.0f;

	float YesRootParameter_ = 0.0f;
	float YesTime = 60;
	float YesBestRotation = 12.0f;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	// デスフラグ
	bool isDead_ = false;

		Hit Hit_ = Hit::NotHit;
	std::optional<Hit> hitRequest_ = std::nullopt;
};
