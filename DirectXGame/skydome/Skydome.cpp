#include "Skydome.h"

void Skydome::Initialize(Model* model) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {0.1f, 0.1f, 0.1f};
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() { 
	
}

void Skydome::Draw(ViewProjection& view) { 
	model_->Draw(worldTransform_, view); }