#include "Rain.h"

void Rain::Initialize(Model* model) { 
	
	assert(model);
	for (int i = 0; i < 5; i++) {
		rainModel[i] = model;
	}
	
	worldTransform_[0].translation_ = {
	    0.0f,100.0f,4.0f
	};
	worldTransform_[1].translation_ = {
		10.0f,150.0f ,4.0f };
	worldTransform_[2].translation_ = {
		50.9f, 120.0f ,4.0f};
	worldTransform_[3].translation_ = {-10.0f, 130.0f, 2.0f};
	worldTransform_[4].translation_ = {-50.9f, 120.0f, -52.0f};



	  for (int i = 0; i < 5; i++) {
		worldTransform_[i].scale_ = {3.0f, 3.0f, 3.0f};
		worldTransform_[i].rotation_ = {0.0f, 10.0f, 0.0f};

		fastTransform_[i].translation_ = worldTransform_[i].translation_;

		worldTransform_[i].Initialize();
	}

}

void Rain::Update() {

	for (int i = 0; i < 5; i++) {

	
		if (target_) {
			//worldTransform_[i].translation_.x = fastTransform_[i].translation_.x + target_->translation_.x;
			worldTransform_[i].translation_.z = fastTransform_[i].translation_.z + target_->translation_.z;
		}


		worldTransform_[i].translation_.y -= 5.0f;
		worldTransform_[i].translation_.x += 4.0f;

		  if (worldTransform_[i].translation_.y <= 0) {
		
			  if (i % 2 == 0) {
			  
			  	  worldTransform_[i].translation_.y = 150.0f;
				  worldTransform_[i].translation_.x = -50.0f;
			  } else
				if(i % 2 != 0)  
			  {
				  worldTransform_[i].translation_.y = 120.0f;
				  worldTransform_[i].translation_.x = -50.0f;
			  }


		 


		  }
		 
		

	}
	for (int i = 0; i < 5; i++) {
		worldTransform_[i].UpdateMatrix();
	
	}

	ImGui::Begin("Rain");
	ImGui::DragFloat3("Position0", &worldTransform_[0].translation_.x, 0.1f);
	ImGui::DragFloat3("Position1", &worldTransform_[1].translation_.x, 0.1f);
	ImGui::DragFloat3("Position2", &worldTransform_[1].translation_.x, 0.1f);
	ImGui::End();

}

void Rain::Draw(ViewProjection& viewProjection) {


	for (int i = 0; i < 5; i++) {
		rainModel[i]->Draw(worldTransform_[i], viewProjection);
	}

}
