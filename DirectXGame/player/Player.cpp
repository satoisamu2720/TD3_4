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
void Player::SunnyUpdate() {

	// 当たった判定
	if (normalHit_ == true) {
		NormalHitMotion();
	}

	move_ = {0, 0, 0};

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move_.x -= kCharacterSpeed;
		LeftMove();
	} else if (input_->PushKey(DIK_D)) {
		move_.x += kCharacterSpeed;
		RightMove();
	} else if (worldTransform_.rotation_.y <= -0.01f ) {
		worldTransform_.rotation_.y += 0.05f;
	} else if (worldTransform_.rotation_.y >= 0.01f ) {
		worldTransform_.rotation_.y -= 0.05f;
	}

#ifdef _DEBUG
	if (input_->PushKey(DIK_W)) {
		move_.z += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move_.z -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_SPACE)) {
		normalHit_ = true;
	}
#endif

	move_ = TransformNormal(move_, MakeRotateYMatrix(viewProjection_->rotation_.y));
	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);


	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformFrontLeft_.UpdateMatrix();
	worldTransformFrontRight_.UpdateMatrix();
	worldTransformBackLeft_.UpdateMatrix();
	worldTransformBackRight_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::Checkbox("normalHit_", &normalHit_);
	ImGui::End();
#endif
}


void Player::ThunderstormUpdate() {
	
	//雷に当たった判定
	if (thunderHit_ == true) {
		ThunderHitMotion();
	}

	move_ = {0, 0, 0};

	// 0だったら左の風
	if (setRand_ == 0) {
		// 左風力0.2
		wind = windLeft;
	}
	//1だったら右の風
	if (setRand_ == 1) {
		// 右風力-0.2
		wind = windRight;
	}

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move_.x -= kCharacterSpeed - wind;
		LeftMove();
	} else if (input_->PushKey(DIK_D)) {
		move_.x += kCharacterSpeed + wind;
		RightMove();
	} else if (worldTransform_.rotation_.y <= -0.01f) {
		worldTransform_.rotation_.y += 0.05f;
	} else if (worldTransform_.rotation_.y >= 0.01f) {
		worldTransform_.rotation_.y -= 0.05f;
	}
	
#ifdef _DEBUG
	if (input_->PushKey(DIK_W)) {
		move_.z += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move_.z -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_SPACE)) {
		thunderHit_ = true;
	}
#endif

	move_ = TransformNormal(move_, MakeRotateYMatrix(viewProjection_->rotation_.y));
	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);



	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformFrontLeft_.UpdateMatrix();
	worldTransformFrontRight_.UpdateMatrix();
	worldTransformBackLeft_.UpdateMatrix();
	worldTransformBackRight_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::Checkbox("thunderHit_", &thunderHit_);
	ImGui::InputFloat("weather", &setRand_);
	ImGui::End();
#endif
}

void Player::NormalHitMotion() {
	const uint16_t period = 120;
	const float step = 2.0f * (float)M_PI / period;
	


	normalHitRootParameter_ += step;
	normalHitRootParameter_ = std::fmod(normalHitRootParameter_, 2.0f * (float)M_PI);

	const float amplitude = 0.1f;

	worldTransform_.rotation_.y += std::sin(normalHitRootParameter_) * amplitude;

	--normalHitTime;

	if (normalHitTime <= 0) {
		worldTransform_.rotation_.y = 0.0f;
		normalHitRootParameter_ = 0.0f;
		normalHitTime = 140;
		normalHit_ = false;
	}
}

void Player::ThunderHitMotion() { 
	

	const uint16_t period = 120;
	const float step = 2.0f * (float)M_PI / period;

	thunderHitRootParameter_ += step;
	thunderHitRootParameter_ = std::fmod(thunderHitRootParameter_, 2.0f * (float)M_PI);

	const float amplitude = 0.1f;

	worldTransformBody_.rotation_.z += std::sin(thunderHitRootParameter_) * amplitude;
	worldTransformBody_.rotation_.z += std::sin(thunderHitRootParameter_) * amplitude;

	worldTransform_.rotation_.y += 0.2f;

	if (worldTransform_.rotation_.y >= 6.0f) {
		worldTransform_.rotation_.y = 0.0f;
		worldTransformBody_.rotation_.z = 0.0f;
		thunderHitRootParameter_ = 0.0f;
		thunderHit_ = false;
	}
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