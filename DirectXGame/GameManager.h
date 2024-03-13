#pragma once
#include "ClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "IScene.h"
#include "SelectScene.h"
#include "TitleScene.h"
#include <memory>

class GameManager {
private:
	std::unique_ptr<IScene> sceneArr_[KSCENEMAX];

	int cureentSceneNo_;
	int preSceneNo_ = NONE;

public:
	GameManager();
	~GameManager();

	// int Run();

	void Check();

	void Update();

	void Draw();
};
