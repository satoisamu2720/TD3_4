#include "Accelerator.h"

void Accelerator::Initialize(Model* model, Vector3 position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {10.0f, 5.0f, 20.0f};
	worldTransform_.rotation_.x = 3.14f;
	worldTransform_.rotation_.y = 3.14f/2.0f;
	worldTransform_.translation_ = position;
}

void Accelerator::Update() {

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("Accelerator");
	ImGui::DragFloat3("Accelerator translation_", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Accelerator Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();

#endif 
}

void Accelerator::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }

Vector3 Accelerator::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};
