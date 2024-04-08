#include "GarbageCan.h"

void GarbageCan::Initialize(Model* model, Vector3 position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.x = -3.14f /2;
	worldTransform_.scale_ = {0.5f, 0.5f, 0.5f};
	input_ = Input::GetInstance();
	
}

void GarbageCan::Update() {

	if (hitRequest_) {
		// 振るまいを変更する
		Hit_ = hitRequest_.value();
		// 各振るまいごとの初期化を実行
		switch (Hit_) {
		case Hit::NotHit:
			NotHitMotionInitialize();
		default:
			break;
		case Hit::YesHit:
			YesHitMotionInitialize();
			break;
		}
		// 振るまいリクエストをリセット
		hitRequest_ = std::nullopt;
	}

	switch (Hit_) {
	case Hit::NotHit:
		NotHitMotion();
	default:
		break;
	case Hit::YesHit:
		YesHitMotion();
		break;
	}


	if (input_->TriggerKey(DIK_L)) {
		worldTransform_.translation_ = {0, 1.5f, 50};
	}

	worldTransform_.UpdateMatrix(); 
}

void GarbageCan::NotHitMotionInitialize() {
	worldTransform_.rotation_ = {-3.14f/2, 0, 0};
	worldTransform_.translation_ = {-16,1,50};
	NotTime = 60;
	NotRootParameter_ = 0.0f;
}

// 障害物に当たった時の更新処理
void GarbageCan::NotHitMotion() {

	const uint16_t period = 60;
	const float step = 2.0f * (float)M_PI /*3.14f*/ / period;

	NotRootParameter_ += step;
	NotRootParameter_ = std::fmod(NotRootParameter_, /*6.28 = */ 2.0f * /*3.14f*/ (float)M_PI);

	const float amplitude = 0.1f;

	worldTransform_.translation_.x += 0.1f;

	worldTransform_.rotation_.z -= 0.1f;

	worldTransform_.translation_.y += std::cos(NotRootParameter_) * amplitude;

	if (rotate) {
	hitRequest_ = Hit::YesHit;
	}

}

void GarbageCan::YesHitMotionInitialize() {
	YesTime = 120;
	YesRootParameter_ = 0.0f;
}

void GarbageCan::YesHitMotion() {
	const uint16_t period = 360;
	const float step = 2.0f * (float)M_PI /*3.14f*/ / period;

	YesRootParameter_ += step;
	YesRootParameter_ = std::fmod(YesRootParameter_, /*6.28 = */ 2.0f * /*3.14f*/ (float)M_PI);

	const float amplitude = 0.1f;

	worldTransform_.translation_.z += 0.8f;

	worldTransform_.rotation_.z -= 0.02f;
	worldTransform_.rotation_.x -= 0.2f;
	//worldTransform_.rotation_.x -= 0.01f;

	worldTransform_.translation_.y += std::cos(YesRootParameter_) * amplitude;

	--YesTime;

	if (YesTime <= 0) {
		hitRequest_ = Hit::NotHit;
		rotate = false;
	}
}

void GarbageCan::SetTranslate(Vector3 translate) { worldTransform_.translation_ = translate; }

void GarbageCan::Draw(ViewProjection& view) { 
	
		model_->Draw(worldTransform_, view); 
}

Vector3 GarbageCan::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
