#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation) {
	input_ = Input::GetInstance();

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	viewProjection_.Initialize();
	viewProjection_.farZ = 5.0f;

	
}

void RailCamera::Update() {
	
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.z += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.z -= kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= cameraSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += cameraSpeed;
	}

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    {1.0f, 1.0f, 1.0f}, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	/*worldTransform_.translation_ = TransformNormal(
	    worldTransform_.translation_, MakeRotateYMatrix(viewProjection_.rotation_.y));*/

	viewProjection_.translation_ = Add(target_->translation_, worldTransform_.translation_);

	ImGui::Begin("Rail Camera");
	ImGui::DragFloat3("Camera Position", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("Camera Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}