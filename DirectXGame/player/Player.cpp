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

	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
}
void Player::Update() {

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformFront_.UpdateMatrix();
	worldTransformL_.UpdateMatrix();
	worldTransformR_.UpdateMatrix();
	worldTransformBack_.UpdateMatrix();
}

void Player::Draw(ViewProjection view) {

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