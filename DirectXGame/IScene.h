#pragma once
#include "Input.h"

enum SceneType {
	NONE,
	TITLE,
	SELECT,
	TEST,
	SUNNYSTAGE,
	CLEAR,
	END,
	KSCENEMAX,
};

class IScene {
protected:
	// シーン番号を管理する変数
	static int sceneNo;

	Input* input_ = nullptr;

public:

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual ~IScene();

	// getter
	int GetSceneNo();

};
