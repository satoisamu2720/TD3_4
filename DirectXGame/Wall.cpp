#include "Wall.h"

void Wall::Initialize(Model* model, Vector3 scele) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = scele;
	worldTransform_.rotation_ = {3.14f/2.0f, 3.14f, 0.0f};
	worldTransform_.translation_ = {0,0,1.5f};
	worldTransform_.UpdateMatrix();
}

void Wall::Update() { worldTransform_.UpdateMatrix(); }

void Wall::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }


Vector3 Wall::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};