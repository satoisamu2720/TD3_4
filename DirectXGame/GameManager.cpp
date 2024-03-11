#include "GameManager.h"

GameManager::GameManager() {

	// �e�V�[���̔z��

	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[SELECT] = std::make_unique<SelectScene>();
	sceneArr_[GAME] = std::make_unique<GameScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	sceneArr_[END] = std::make_unique<GameOverScene>();

	cureentSceneNo_ = TITLE;
}

GameManager::~GameManager() {}

void GameManager::Run() {

	//// �Q�[�����[�v
	// while (true) {
	//
	// }

	// �V�[���̃`�F�b�N
	preSceneNo_ = cureentSceneNo_;
	cureentSceneNo_ = sceneArr_[cureentSceneNo_]->GetSceneNo();

	// �V�[���̕ύX�`�F�b�N
	if (preSceneNo_ != cureentSceneNo_) {
		sceneArr_[cureentSceneNo_]->Initialize();
	}

	// �V�[���̍X�V����
	sceneArr_[cureentSceneNo_]->Update();

	// �V�[���̕`�揈��
	sceneArr_[cureentSceneNo_]->Draw();

}
