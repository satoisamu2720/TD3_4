#include "Player.h"

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();
	// 初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformFront_.Initialize();
	worldTransformBack_.Initialize();

	// 初期ポジティブ
	worldTransform_.translation_ = position;
	worldTransformBody_.translation_ = bodyPosition;
	worldTransformFront_.translation_ = frontPosition;
	worldTransformBack_.translation_ = backPosition;

	// 親子関係
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformFront_.parent_ = &worldTransformBody_;
	worldTransformBack_.parent_ = &worldTransformBody_;

	worldTransform_.scale_ = {10.0f, 10.0f, 10.0f};

	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
}

void Player::Update() {

	if (weatherHitRequest_) {
		// 振るまいを変更する
		weatherHit_ = weatherHitRequest_.value();
		// 各振るまいごとの初期化を実行
		switch (weatherHit_) {
		case Player::WeatherHit::NotHit:
			NotHitInitialize();
		default:
			break;
		case Player::WeatherHit::Normal:
			NormalHitMotionInitialize();
			break;
		case Player::WeatherHit::Thunder:
			ThunderHitMotionInitialize();
			break;
		}
		// 振るまいリクエストをリセット
		weatherHitRequest_ = std::nullopt;
	}

	switch (weatherHit_) {
	case Player::WeatherHit::NotHit:
	default:

		break;
	case Player::WeatherHit::Normal:
		NormalHitMotion();
		break;
	case Player::WeatherHit::Thunder:
		ThunderHitMotion();
		break;
	}

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformFront_.UpdateMatrix();
	worldTransformBack_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("Player SunnyUpdate");
	ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::Checkbox("normalHit_", &normalHit_);
	ImGui::End();

	ImGui::Begin("Player ThunderstormUpdate");
	ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::InputFloat("bestRotation", &bestRotation,1.0f);
	ImGui::Checkbox("thunderHit_", &thunderHit_);
	ImGui::InputFloat("weather", &setRand_, 1.0f);
	ImGui::End();
#endif
}

void Player::NotHitInitialize() {

	// worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.rotation_.x = 0.0f;
	worldTransformFront_.rotation_.y = 0.0f;
	worldTransform_.rotation_.z = 0.0f;
	worldTransformBody_.rotation_ = {0.0f, 0.0f, 0.0f};
}
void Player::SunnyUpdate() {

	// 当たった判定
	if (normalHit_ == true) {
		notRotate = true;
		weatherHitRequest_ = WeatherHit::Normal;
		normalHit_ = false;
	}

	move_ = {0, 0, 0};

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move_.x -= kCharacterSpeed;
		if (notRotate == false) {
			LeftMove();
		}
	} else if (input_->PushKey(DIK_D)) {
		move_.x += kCharacterSpeed;
		if (notRotate == false) {
			RightMove();
		}
	} else if (worldTransform_.rotation_.y <= -0.05f && notRotate == false) {
		worldTransform_.rotation_.y += 0.05f;
		worldTransformFront_.rotation_.y += 0.025f;
	} else if (worldTransform_.rotation_.y >= 0.05f && notRotate == false) {
		worldTransform_.rotation_.y -= 0.05f;
		worldTransformFront_.rotation_.y -= 0.025f;
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
}

void Player::ThunderstormUpdate() {

	// 雷に当たった判定
	if (thunderHit_ == true) {
		notRotate = true; 
		weatherHitRequest_ = WeatherHit::Thunder;
		thunderHit_ = false;
	}

	move_ = {0, 0, 0};

	// 0だったら左の風
	if (setRand_ == 0) {
		// 左風力0.2
		wind = windLeft;
	}
	// 1だったら右の風
	if (setRand_ == 1) {
		// 右風力-0.2
		wind = windRight;
	}

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move_.x -= kCharacterSpeed - wind;
		if (notRotate == false) {
			LeftMove();
		}
	} else if (input_->PushKey(DIK_D)) {
		move_.x += kCharacterSpeed + wind;
		if (notRotate == false) {
			RightMove();
		}
	} else if (worldTransform_.rotation_.y <= -0.05f && notRotate == false) {
		worldTransform_.rotation_.y += 0.05f;
		worldTransformFront_.rotation_.y += 0.025f;
	} else if (worldTransform_.rotation_.y >= 0.05f && notRotate == false) {
		worldTransform_.rotation_.y -= 0.05f;
		worldTransformFront_.rotation_.y -= 0.025f;
	}
	if (worldTransform_.rotation_.y >= bestRotation) {
		worldTransform_.rotation_.y = 0.0f;
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
}

#pragma region 当たった時のモーション

// 障害物に当たった時の初期化
void Player::NormalHitMotionInitialize() {
	worldTransform_.rotation_.y = 0.0f;
	normalHitRootParameter_ = 0.0f;
	normalHitTime = 120;
}
// 障害物に当たった時の更新処理
void Player::NormalHitMotion() {
	const uint16_t period = 120;
	const float step = 2.0f * (float)M_PI /*3.14f*/ / period;

	normalHitRootParameter_ += step;
	normalHitRootParameter_ =
	    std::fmod(normalHitRootParameter_, /*6.28 = */ 2.0f * /*3.14f*/ (float)M_PI);

	const float amplitude = 0.2f;

	worldTransform_.rotation_.y += std::cos(normalHitRootParameter_) * amplitude;


	--normalHitTime;

	if (normalHitTime <= 0) {
		weatherHitRequest_ = WeatherHit::NotHit;
		notRotate = false;
	}
}

// 雷に当たった時の初期化
void Player::ThunderHitMotionInitialize() {
	worldTransform_.rotation_.y = 0.0f;
	worldTransformBody_.rotation_.z = 0.0f;
	worldTransformBody_.rotation_.y = 0.0f;
	thunderHitRootParameter_ = 0.0f;
}
// 雷に当たった時の更新処理
void Player::ThunderHitMotion() {

	const uint16_t period = 30;
	const float step = 2.0f * (float)M_PI /*3.14f*/ / period;

	thunderHitRootParameter_ += step;
	thunderHitRootParameter_ =
	    std::fmod(thunderHitRootParameter_, /*6.28 = */ 2.0f * /*3.14f*/ (float)M_PI);

	const float amplitude = 0.2f;

	//worldTransformBody_.rotation_.x += std::cos(thunderHitRootParameter_) * amplitude;
	worldTransformBody_.rotation_.z += std::cos(thunderHitRootParameter_) * amplitude;

	worldTransform_.rotation_.y += 0.2f;

	if (worldTransform_.rotation_.y >= bestRotation) {
		weatherHitRequest_ = WeatherHit::NotHit;
		notRotate = false;
	}
}

#pragma endregion

void Player::Draw(ViewProjection& view) {

	models_[0]->Draw(worldTransformBody_, view);
	models_[1]->Draw(worldTransformFront_, view);
	models_[2]->Draw(worldTransformBack_, view);
}



Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};

void Player::LeftMove() {
	if (worldTransform_.rotation_.y >= -0.5f) {
		worldTransform_.rotation_.y -= 0.05f;
		worldTransformFront_.rotation_.y -= 0.025f;
	}
}

void Player::RightMove() {
	if (worldTransform_.rotation_.y <= 0.5f) {
		worldTransform_.rotation_.y += 0.05f;
		worldTransformFront_.rotation_.y  += 0.025f;
	}
}
