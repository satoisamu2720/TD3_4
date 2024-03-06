#include "Player.h"

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();
	// 初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformFront_.Initialize();
	worldTransformL_.Initialize();
	worldTransformR_.Initialize();
	worldTransformBack_.Initialize();

	// 初期ポジティブ
	worldTransform_.translation_ = position;
	worldTransformBody_.translation_ = bodyPosition;
	worldTransformFront_.translation_ = frontPosition;
	worldTransformL_.translation_ = lPosition;
	worldTransformR_.translation_ = rPosition;
	worldTransformBack_.translation_ = backPosition;

	// 親子関係
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformFront_.parent_ = &worldTransformBody_;
	worldTransformL_.parent_ =	&worldTransformBody_;
	worldTransformR_.parent_ = &worldTransformBody_;
	worldTransformBack_.parent_ = &worldTransformBody_;

	worldTransform_.scale_ = {10.0f, 10.0f, 10.0f};

	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
}
void Player::Update() {

	move_ = {0, 0, 0};

	
	//// const float kRotSpeed = 0.05f;
	//if (input_->PushKey(DIK_W)) {
	//	move_.z += kCharacterSpeed;
	//} else if (input_->PushKey(DIK_S)) {
	//	move_.z -= kCharacterSpeed;
	//}
	//// 押した方向で移動ベクトルを変更（左右）
	//if (input_->PushKey(DIK_A)) {
	//	move_.x -= kCharacterSpeed;
	//} else if (input_->PushKey(DIK_D)) {
	//	move_.x += kCharacterSpeed;
	//}

	//move_ = TransformNormal(move_, MakeRotateYMatrix(viewProjection_->rotation_.y));
	// Y軸周り角度
	worldTransform_.rotation_.y = std::atan2(move_.x, move_.z);
	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_); 

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformFront_.UpdateMatrix();
	worldTransformL_.UpdateMatrix();
	worldTransformR_.UpdateMatrix();
	worldTransformBack_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

void Player::Draw(ViewProjection &view) {
	
	models_[0]->Draw(worldTransformBody_, view);
	models_[1]->Draw(worldTransformFront_, view);
	models_[2]->Draw(worldTransformL_, view);
	models_[3]->Draw(worldTransformR_, view);
	models_[4]->Draw(worldTransformBack_, view);
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};