#pragma once
#include <Model.h>
#include <vector>
#include "WorldTransform.h"

class BaseCharacter {
protected:
	// ���f���f�[�^�z��
	std::vector<Model*> models_;
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

public:
	virtual void Initialize(const std::vector<Model*>& models);
	virtual void Update();
	virtual void Draw( ViewProjection& view);
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
};