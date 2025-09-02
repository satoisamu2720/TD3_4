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

	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
	worldTransform_.UpdateMatrix();
}
void Player::Update() {}
