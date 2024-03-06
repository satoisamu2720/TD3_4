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
	
	
	Vector3 move_ = {0, 0, 0};

	//move_.z += 1.0f;

	if (input_->PushKey(DIK_R)) {
		move_ = {0.0f, 0.0f, -15.0f};
	}

	if (input_->PushKey(DIK_UP)) {
		move_.z += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move_.z -= kCharacterSpeed;
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
	// ベクターの加算
	viewProjection_.translation_ = Add(worldTransform_.translation_, worldTransform_.rotation_); 

	 move_ = TransformNormal(move_, MakeRotateYMatrix(worldTransform_.rotation_.y));
	 // ベクターの加算
	 worldTransform_.translation_ = Add(worldTransform_.translation_, move_); 

	ImGui::Begin("Rail Camera");
	ImGui::DragFloat3("Camera Position", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("Camera Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Camera Rotation viewProjection_", &viewProjection_.rotation_.y, 0.01f);
	ImGui::End();
}