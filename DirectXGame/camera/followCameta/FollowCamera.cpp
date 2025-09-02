#include "FollowCamera.h"

void FollowCamera::Initialize() { 
	viewProjection_.Initialize();
	input_ = Input::GetInstance();
}

void FollowCamera::Update() {
	// 追従対象からカメラまでのオフセット
	Vector3 offset_ = {0.0f, 1.0f, 3.0f};
	if (target_) {
		float kRotSpeed = 0.02f;
		if (input_->PushKey(DIK_LEFT)) {
		} else if (input_->PushKey(DIK_RIGHT)) {
			viewProjection_.rotation_.y += kRotSpeed;
		}
		if (viewProjection_.rotation_.y <= 3.14f / 2) {
			viewProjection_.rotation_.y -= kRotSpeed;
		}
		rotateYMatrix_ = MakeRotateYmatrix(viewProjection_.rotation_.y);
		offset_ = TransformNormal(offset_, rotateYMatrix_);
		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset_);
	}
		viewProjection_.UpdateMatrix();
}
