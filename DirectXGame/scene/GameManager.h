#pragma once
#include "scene/ClearScene.h"
#include "scene/GameOverScene.h"
#include "scene/GameScene.h"
#include "scene/IScene.h"
#include "scene/SelectScene.h"
#include "scene/TitleScene.h"
#include "scene/SunnyStage.h"
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
