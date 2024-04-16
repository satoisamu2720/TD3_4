#include "Rain.h"

void Rain::Initalize(const std::vector<Model*>& models) { 
	BaseCharacter::Initialize(models);

	
	worldTransform_[0].translation_ = {0.0f, 4.0f, 100.0f};

	worldTransform_[1].translation_ = {10.0f, 4.0f, 150.0f};

	worldTransform_[2].translation_ = {50.9f, 4.0f, 120.0f};




	  for (int i = 0; i < 3; i++) {
		worldTransform_[i].scale_ = {3.0f, 3.0f, 3.0f};
		worldTransform_[i].rotation_ = {0.0f, 10.0f, 0.0f};

		fastTransform_[i].translation_ = worldTransform_[i].translation_;

		worldTransform_[i].Initialize();
	}

}

void Rain::Update() {

	for (int i = 0; i < 3; i++) {

	
		if (target_) {
			worldTransform_[i].translation_.x = fastTransform_[i].translation_.x + target_->translation_.x;
			worldTransform_[i].translation_.y = fastTransform_[i].translation_.y + target_->translation_.y;
		}


		worldTransform_[i].translation_.z -= 0.01f;

		  if (worldTransform_[i].translation_.z <= 0) {
		
		  worldTransform_[i].translation_.z = 100.0f;
		  }
		 
		

	}
	for (int i = 0; i < 3; i++) {
		worldTransform_[i].UpdateMatrix();
	
	}

	ImGui::Begin("Rain");
	ImGui::DragFloat3("Position0", &worldTransform_[0].translation_.x, 0.1f);
	ImGui::DragFloat3("Position1", &worldTransform_[1].translation_.x, 0.1f);
	ImGui::DragFloat3("Position2", &worldTransform_[1].translation_.x, 0.1f);
	ImGui::End();

}

void Rain::Draw(ViewProjection& viewProjection) {


	for (int i = 0; i < 3; i++) {
		models_[i]->Draw(worldTransform_[i], viewProjection);
	}

}
