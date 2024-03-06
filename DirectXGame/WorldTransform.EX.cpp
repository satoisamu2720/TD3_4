#include "WorldTransform.h"
#include "VectraCalculation.h"
void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	//cameraMatWorld_ = CameraMakeAffineMatrix(scale_, rotation_, translation_);

	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}
	/*if (cameraParent_) {
		cameraMatWorld_ = Multiply(cameraMatWorld_, cameraParent_->cameraMatWorld_);
	}*/
	TransferMatrix();
}


