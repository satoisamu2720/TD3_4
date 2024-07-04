#include "DownPanel.h"

void DownPanel::Initialize(Model* model, Vector3 position) {

	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void DownPanel::Update() { worldTransform_.UpdateMatrix(); }

void DownPanel::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }

void DownPanel::OnCollision() { isDead_ = true; }

Vector3 DownPanel::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
