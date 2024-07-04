#include "SnowPool.h"

void SnowPool::Initialize(Model* model, Vector3 position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void SnowPool::Update() { worldTransform_.UpdateMatrix(); }

void SnowPool::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }

void SnowPool::OnCollision() { isDead_ = true; }

Vector3 SnowPool::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
