#pragma once
	#pragma once

#include "ImGuiManager.h"
#include "VectraCalculation.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "input.h"
#include <memory>
#include "Sprite.h"

class Timer {

	public:
		void Initialize();
	    void Update(bool flag);

		void Draw();

		void SetTime(float math, float second);

	private:
		const WorldTransform* target_ = nullptr;
		Input* input_ = nullptr;

		float MathTime = 0.0f;
	
		float SecondTime = 0.0f;

		float wTime = 0.0f;

		bool isFlag = false;

	Sprite *texHandle[10] = {};



};
