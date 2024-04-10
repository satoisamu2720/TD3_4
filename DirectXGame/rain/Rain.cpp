#include "Rain.h"

void Rain::Initalize(const std::vector<Model*>& models) { 
	BaseCharacter::Initialize(models);

	  for (int i = 0; i < 20; i++) {

		
		worldTransform_[i].Initialize();
	}

}

void Rain::Update() {

	

	for (int i = 0; i < 20; i++) {

		worldTransform_[i].translation_.z -= 0.1f;

		  if (worldTransform_[i].translation_.z <= 0) {
		
		  worldTransform_[i].translation_.z = 100.0f;
		  }



		worldTransform_[i].UpdateMatrix();
	}
}

void Rain::Draw(ViewProjection& viewProjection) {


	for (int i = 0; i < 20; i++) {
		models_[i]->Draw(worldTransform_[i], viewProjection);
	}

}
