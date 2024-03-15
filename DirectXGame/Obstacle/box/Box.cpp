#include "Box.h"

void Box::Initialize(Model* model) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
}

void Box::Update() { worldTransform_.UpdateMatrix(); }

void Box::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }


Vector3 Box::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};
