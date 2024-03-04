#pragma once
#include <Model.h>
#include <vector>
class BaseCharacter {
protected:
	// モデルデータ配列
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldTransform_;

public:
	virtual void Initialize(const std::vector<Model*>& models);
	virtual void Update();
	virtual void Draw(const ViewProjection& view);
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
};
