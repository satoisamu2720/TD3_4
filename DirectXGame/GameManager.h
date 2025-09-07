#pragma once
#include "IScene.h"
#include "ClearScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "EndScene.h"

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
