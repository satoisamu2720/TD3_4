#include "Cloud.h"

void Cloud::Initialize(Model* model,bool flag) {
	assert(model);
	model_ = model;
	selectFlag = flag;
	for (int i = 0; i < 4; i++) {
	worldTransform_[i].Initialize();
	worldTransform_[i].scale_ = {4.0f, 4.0f, 4.0f};

	}
	if (selectFlag == false) {
	worldTransform_[0].translation_ = {-40,10,-21};
	worldTransform_[1].translation_ = {-40,-10,-21};
	worldTransform_[2].translation_ = {40, 5,-21};
	worldTransform_[3].translation_ = {40,-15,-21};
	}
	if (selectFlag == true) {
	worldTransform_[0].translation_ = {-2, 10, -21};
	worldTransform_[1].translation_ = {-2, -10, -21};
	worldTransform_[2].translation_ = {2, 5, -21};
	worldTransform_[3].translation_ = {2, -15, -21};
	}
}

void Cloud::Update() {
	if (selectFlag == false && moveFlag) {
	worldTransform_[0].translation_.x += moveSpeed;
	worldTransform_[1].translation_.x += moveSpeed;
	worldTransform_[2].translation_.x -= moveSpeed;
    worldTransform_[3].translation_.x -= moveSpeed;
	}

	if (selectFlag == true && moveFlag) {
	worldTransform_[0].translation_.x -= moveSpeed;
	worldTransform_[1].translation_.x -= moveSpeed;
	worldTransform_[2].translation_.x += moveSpeed;
	worldTransform_[3].translation_.x += moveSpeed;
	}
	for (int i = 0; i < 4; i++) {
	worldTransform_[i].UpdateMatrix();
	}
}

void Cloud::Draw(ViewProjection& view) {
	for (int i = 0; i < 4; i++) {
	model_->Draw(worldTransform_[i], view);
	}
}