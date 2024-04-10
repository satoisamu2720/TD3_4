#include "Rain.h"

void Rain::Initalize(const std::vector<Model*>& models) { 
	BaseCharacter::Initialize(models);

	worldTransform_[0].scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_[0].rotation_ = {0.0f, 10.0f, 0.0f};
	worldTransform_[0].translation_ = {0.0f, 4.0f, 100.0f};

	worldTransform_[1].scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_[1].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_[1].translation_ = {10.0f, 4.0f, 150.0f};

	worldTransform_[2].scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_[2].rotation_ = {0.0f, -2.0f, 0.0f};
	worldTransform_[2].translation_ = {50.9f, 4.0f, 120.0f};




	  for (int i = 0; i < 20; i++) {







		
		worldTransform_[i].Initialize();
	}

}

void Rain::Update() {

	





	for (int i = 0; i < 20; i++) {


		worldTransform_[i].translation_.x += target_->translation_.x;
		worldTransform_[i].translation_.y += target_->translation_.y;




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
