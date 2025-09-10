#include "Player.h"

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();
	// 初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformLight_.Initialize();


	// 初期ポジティブ
	worldTransform_.translation_ = position;
	worldTransformBody_.translation_ = bodyPosition;
	worldTransformLight_.translation_ = lightPosition_;

	// 親子関係
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformLight_.parent_ = &worldTransform_;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	worldTransformLight_.rotation_.z = 1.57f;

	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
	worldTransform_.UpdateMatrix();
}

void Player::Update() {

	move_ = {0, 0, 0};

	move_.y -= Gravity;
	// キー入力で移動
	if (input_->PushKey(DIK_A)) {
		move_.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move_.x += kCharacterSpeed;
	}
	/*if (input_->PushKey(DIK_S)) {
		move_.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move_.y += kCharacterSpeed;
	}*/
	if (!mirror && gameStart) {
		if (input_->IsTriggerMouse(0) && !lightFlag) {
			lightFlag = true;
			lightCount--;
		}
	}

	/*if (input_->PushKey(DIK_SPACE))
	{
		move_.y =+ JumpSpeed;
	}*/

	if (lightFlag) {
		lightTimer++;
	}
	if (lightTimer >= 60) {
		lightTimer = 0;
		lightFlag = false;
	}
	// --- マウス位置に応じてライト位置を決定 ---
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(GetActiveWindow(), &mousePos);

	int screenWidth = 1280; // ウィンドウサイズに置き換え
	int screenHeight = 720;

	// マウス座標を[-1,1]に正規化
	float mouseX = (mousePos.x - screenWidth / 2.0f) / (screenWidth / 2.0f);
	float mouseY = (mousePos.y - screenHeight / 2.0f) / (screenHeight / 2.0f);
	mouseY = -mouseY; // 上下反転

	// 方向ベクトル（XZ平面ベース）
	Vector3 lightDir = {mouseX, 0.0f, mouseY};
	float len = sqrtf(lightDir.x * lightDir.x + lightDir.z * lightDir.z);
	if (len > 0.0001f) {
		lightDir.x /= len;
		lightDir.z /= len;
	}

	// プレイヤーからの距離
	float lightDistance = 2.5f;
	lightDir.x *= lightDistance;
	lightDir.z *= lightDistance;

	// カメラ回転を反映
	float cameraYaw = viewProjection_->rotation_.y;
	Matrix4x4 camRotMat = MakeRotateYmatrix(cameraYaw);
	lightDir = TransformNormal(lightDir, camRotMat);

	// ライトの相対位置を設定
	worldTransformLight_.translation_ = lightDir;
	// ---- 向きの計算 ----
	Vector3 playerPos = {
	    worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	    worldTransform_.matWorld_.m[3][2]};

	Vector3 lightWorldPos = Add(playerPos, lightDir);
	Vector3 dir = {
	    lightWorldPos.x - playerPos.x, lightWorldPos.y - playerPos.y,
	    lightWorldPos.z - playerPos.z};


	if (dir.x > 0) {
		worldTransformLight_.rotation_.x = 3.14f;
	} else {
		worldTransformLight_.rotation_.x = 0.0f;
	}

	// --- 移動処理 ---
	move_ = TransformNormal(move_, MakeRotateYmatrix(viewProjection_->rotation_.y));
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformLight_.UpdateMatrix();


	

#ifdef _DEBUG
	if (!lightFlag) {
		ImGui::Begin("Player SunnyUpdate");
		ImGui::DragInt("Light Count", &lightCount, 0.1f);
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
	if (lightFlag) {
		models_[1]->Draw(worldTransformLight_, view);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};

Vector3 Player::GetLightWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransformLight_.matWorld_.m[3][0];
	worldPos.y = worldTransformLight_.matWorld_.m[3][1];
	worldPos.z = worldTransformLight_.matWorld_.m[3][2];

	return worldPos;
};
