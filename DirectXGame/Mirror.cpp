#include "Mirror.h"


void Mirror::Initialize(Model* model, Vector3 position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;
}

void Mirror::Update() { worldTransform_.UpdateMatrix(); }

void Mirror::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }

void Mirror::SetTranslate(Vector3 translate) { worldTransform_.translation_ = translate; }

Vector3 Mirror::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};