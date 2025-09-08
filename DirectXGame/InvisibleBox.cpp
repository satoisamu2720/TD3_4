#include "InvisibleBox.h"


void InvisibleBox::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;

}

void InvisibleBox::Update() { worldTransform_.UpdateMatrix(); }

void InvisibleBox::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }

void InvisibleBox::SetTranslate(Vector3 translate) { worldTransform_.translation_ = translate; }

Vector3 InvisibleBox::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};
