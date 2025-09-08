#include "IScene.h"

int IScene::sceneNo = SUNNYSTAGE;

IScene::~IScene() {}

int IScene::GetSceneNo() { return sceneNo; }
