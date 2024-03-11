#pragma once
#include "IScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include <memory>

class GameManager {
private:
	std::unique_ptr<IScene> sceneArr_[5];

	int cureentSceneNo_;
	int preSceneNo_;

public:

	GameManager();
	~GameManager();

	void Run();


};
