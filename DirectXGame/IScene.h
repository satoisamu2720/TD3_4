#pragma once
#include "Input.h"

enum SceneType {
	TITLE,
	SELECT,
	GAME,
	CLEAR,
	END,
	KSCENEMAX,
};

class IScene {
protected:
	// �V�[���ԍ����Ǘ�����ϐ�
	static int sceneNo;

	Input* input_ = nullptr;

public:

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual ~IScene();

	// getter
	int GetSceneNo() { return sceneNo; }

};
