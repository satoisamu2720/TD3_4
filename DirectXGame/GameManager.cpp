#include "GameManager.h"

GameManager::GameManager() {

	// 各シーンの配列

	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[SELECT] = std::make_unique<SelectScene>();
	sceneArr_[GAME] = std::make_unique<GameScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	sceneArr_[END] = std::make_unique<GameOverScene>();

	cureentSceneNo_ = TITLE;
}

GameManager::~GameManager() {}

void GameManager::Run() {

	//// ゲームループ
	// while (true) {
	//
	// }

	// シーンのチェック
	preSceneNo_ = cureentSceneNo_;
	cureentSceneNo_ = sceneArr_[cureentSceneNo_]->GetSceneNo();

	// シーンの変更チェック
	if (preSceneNo_ != cureentSceneNo_) {
		sceneArr_[cureentSceneNo_]->Initialize();
	}

	// シーンの更新処理
	sceneArr_[cureentSceneNo_]->Update();

	// シーンの描画処理
	sceneArr_[cureentSceneNo_]->Draw();

}
