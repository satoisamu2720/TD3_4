#include "Player.h"

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();
	// 初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();

	// 初期ポジティブ
	worldTransform_.translation_ = position;
	worldTransformBody_.translation_ = bodyPosition;

	// 親子関係
	worldTransformBody_.parent_ = &worldTransform_;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
	worldTransform_.UpdateMatrix();
}

void Player::Update() {
	
		move_ = {0, 0, 0};
		

		//
		//XINPUT_STATE joyState;

		//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//	// 押した方向で移動ベクトルを変更（左右）
		//	if ( joyState.Gamepad.sThumbLX < -100) {
		//		move_.x -= kCharacterSpeed;
		//		if (notRotate == false) {
		//			LeftMove();
		//		}
		//	}else if ( joyState.Gamepad.sThumbLX > 100) {
		//		move_.x += kCharacterSpeed;
		//		if (notRotate == false) {
		//			RightMove();
		//		}
		//	} else if (worldTransform_.rotation_.y <= -0.05f && notRotate == false) {
		//		worldTransform_.rotation_.y += 0.05f;
		//		worldTransformFront_.rotation_.y += 0.025f;
		//	} else if (worldTransform_.rotation_.y >= 0.05f && notRotate == false) {
		//		worldTransform_.rotation_.y -= 0.05f;
		//		worldTransformFront_.rotation_.y -= 0.025f;
		//	}
		//	
		//} 
		//

		// 押した方向で移動ベクトルを変更（左右）
		if (input_->PushKey(DIK_A)) {
			    move_.x -= kCharacterSpeed;
			    
		}
		else if (input_->PushKey(DIK_D)) {
			    move_.x += kCharacterSpeed;
			   
		} 
			
		
		
		//worldTransformFront_.rotation_.x += 0.01f;

#ifdef _DEBUG
		if (input_->PushKey(DIK_W)) {
			move_.z += kCharacterSpeed;
		} else if (input_->PushKey(DIK_S)) {
			move_.z -= kCharacterSpeed;
		}
		
#endif
	
	move_ = TransformNormal(move_, MakeRotateYmatrix(viewProjection_->rotation_.y));
	    // Y軸周り角度
	    worldTransform_.rotation_.y = std::atan2(move_.x, move_.z);
	    // ベクターの加算
	    worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
	    // 行列更新
	    worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();

#ifdef _DEBUG
	if (weather_ == 0) {
		ImGui::Begin("Player SunnyUpdate");
		ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.1f);
		ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
		
		ImGui::End();
	}

	if (weather_ == 1) {
		ImGui::Begin("Player ThunderstormUpdate");
		ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.1f);
		ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
		ImGui::End();
	}
#endif
}


void Player::Draw(ViewProjection& view) {

	models_[0]->Draw(worldTransformBody_, view);
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};

