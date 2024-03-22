#include "Skydome.h"

void Skydome::Initialize(Model* model, Vector3 position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 5.0f};
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }