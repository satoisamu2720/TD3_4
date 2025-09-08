#include "FollowCamera.h"

    void
    FollowCamera::Initialize(const Vector3& position, const Vector3& rotation) {

	viewProjection_.Initialize();
	viewProjection_.translation_ = position;
	viewProjection_.rotation_ = rotation;

	input_ = Input::GetInstance();
}

void FollowCamera::Update() {

	// 追従対象からカメラまでのオフセット
	Vector3 offset = {0.0f, 0.0f, -20.0f};

	if (target_) {

		float kRotSpeed = 0.05f;

		// 正面/裏切替
		if (!Inversion) {
			viewProjection_.rotation_.y = 0.0f; // 右横
		} else {
			viewProjection_.rotation_.y = 3.14f; // 左横
		}

		// 上下方向の微調整
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.rotation_.y += kRotSpeed;
		} else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.rotation_.y -= kRotSpeed;
		}

		// Y回転を反映してオフセットを回転
		rotateYMatrix_ = MakeRotateYmatrix(viewProjection_.rotation_.y);
		Vector3 rotatedOffset = TransformNormal(offset, rotateYMatrix_);

		// カメラ位置を毎フレームリセット
		viewProjection_.translation_ = Add(target_->translation_, rotatedOffset);

		// ワールド行列・ビュー行列更新
		worldTransform_.matWorld_ = MakeAffineMatrix(
		    {1.0f, 1.0f, 1.0f}, viewProjection_.rotation_, viewProjection_.translation_);
		viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	}

	viewProjection_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("offset Camera");
	ImGui::DragFloat3("Camera Position", &viewProjection_.translation_.x, 0.01f);
	ImGui::DragFloat3("Camera Rotation", &viewProjection_.rotation_.x, 0.01f);
	ImGui::End();
#endif
}
