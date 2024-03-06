#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"


class Box {
public:

	void Initilize(Model*model);
	void Updata();
	void Draw(ViewProjection &view);

private:

	Model* boxModel_ = nullptr;
	WorldTransform worldTransform_ = {};






};
