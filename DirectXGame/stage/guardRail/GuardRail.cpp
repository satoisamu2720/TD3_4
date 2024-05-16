#include "GuardRail.h"

void TrafficLight::Initialize(Model* model, Vector3 position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.UpdateMatrix();
}

void TrafficLight::Update() {}

void TrafficLight::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }
