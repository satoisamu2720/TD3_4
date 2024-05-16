#pragma once
class Cloud {
public:
	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(ViewProjection& view);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

public:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	// デスフラグ
	bool isDead_ = false;
};
