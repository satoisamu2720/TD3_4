#include "Player.h"

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();
	// 初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformFrontLeft_.Initialize();
	worldTransformFrontRight_.Initialize();
	worldTransformBackLeft_.Initialize();
	worldTransformBackRight_.Initialize();

	// 初期ポジティブ
	worldTransform_.translation_ = position;
	worldTransformBody_.translation_ = bodyPosition;
	worldTransformFrontLeft_.translation_ = frontLeftPosition;
	worldTransformFrontRight_.translation_ = frontRightPosition;
	worldTransformBackLeft_.translation_ = backLeftPosition;
	worldTransformBackRight_.translation_ = backRightPosition;

	// 親子関係
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformFrontLeft_.parent_ = &worldTransformBody_;
	worldTransformFrontRight_.parent_ = &worldTransformBody_;
	worldTransformBackLeft_.parent_ = &worldTransformBody_;
	worldTransformBackRight_.parent_ = &worldTransformBody_;

	worldTransform_.scale_ = {10.0f, 10.0f, 10.0f};

	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
}
void Player::Update() {

	move_ = {0, 0, 0};

	
	// const float kRotSpeed = 0.05f;
	if (input_->PushKey(DIK_W)) {
		move_.z += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move_.z-= kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move_.x -= kCharacterSpeed;
		LeftMove();
	} else if (input_->PushKey(DIK_D)) {
		move_.x += kCharacterSpeed;
		RightMove();
	} 
	else if (worldTransform_.rotation_.y <= -0.01f) {
		worldTransform_.rotation_.y += 0.05f;
	} 
	else if (worldTransform_.rotation_.y >= 0.01f) {
		worldTransform_.rotation_.y -= 0.05f;
	}

	move_ = TransformNormal(move_, MakeRotateYMatrix(viewProjection_->rotation_.y));
	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_); 

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformFrontLeft_.UpdateMatrix();
	worldTransformFrontRight_.UpdateMatrix();
	worldTransformBackLeft_.UpdateMatrix();
	worldTransformBackRight_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
	//ImGui::DragFloat3("Player Rotation", &viewProjection_->rotation_.y, 0.01f);
	ImGui::End();
}

void Player::Draw(ViewProjection &view) {
	
	models_[0]->Draw(worldTransformBody_, view);
	/*models_[1]->Draw(worldTransformFrontLeft_, view);
	models_[2]->Draw(worldTransformFrontRight_, view);
	models_[3]->Draw(worldTransformBackLeft_, view);
	models_[4]->Draw(worldTransformBackRight_, view);*/
}


Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};

void Player::RightMove() { 
	if (worldTransform_.rotation_.y <= 0.5f) {
	worldTransform_.rotation_.y += 0.05f; 
	}
}

void Player::LeftMove() {
	if (worldTransform_.rotation_.y >=- 0.5f) {
	worldTransform_.rotation_.y -= 0.05f; 
	}
}