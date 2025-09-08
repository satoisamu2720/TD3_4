#include "Player.h"

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();
	// 初期化
	worldTransform_.Initialize();
	// 初期ポジティブ
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
}
void Player::Update() {
	move_ = {0, 0, 0};

	if (input_->PushKey(DIK_W)) {
		move_.z += Speed;
	} else if (input_->PushKey(DIK_S)) {
		move_.z -= Speed;
	}
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move_.x -= Speed;
	} else if (input_->PushKey(DIK_D)) {
		move_.x += Speed;
	} 
	move_ = TransformNormal(move_, MakeRotateYmatrix(viewProjection_->rotation_.y));
	// Y軸周り角度
	worldTransform_.rotation_.y = std::atan2(move_.x, move_.z);
	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
	// 行列更新
	worldTransform_.UpdateMatrix();
#ifdef _DEBUG
	float imputFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	// デバッグ
	ImGui::Begin("Debug");
	ImGui::SliderFloat3("player", imputFloat3, -30.0f, 30.0f);
	ImGui::End();
	worldTransform_.translation_.x = imputFloat3[0];
	worldTransform_.translation_.y = imputFloat3[1];
	worldTransform_.translation_.z = imputFloat3[2];
#endif
}


void Player::Draw(ViewProjection& view) {

	models_[0]->Draw(worldTransform_, view);
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};

Vector3 Player::GetTransformRotation() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.rotation_.x;
	worldPos.y = worldTransform_.rotation_.y;
	worldPos.z = worldTransform_.rotation_.z;

	return worldPos;
};

Vector3 Player::GetTransformPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
};