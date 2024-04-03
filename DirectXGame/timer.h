#pragma once
#include "ImGuiManager.h"
#include "Sprite.h"
#include "VectraCalculation.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "input.h"
#include <memory>

class Timer {

public:
	void Initialize();
	void Update();

	void Draw();

	void SetTime(int math, int second);

	int GetTimeMath() { return mathTime; }
	int  GetTimeSecond() { return secondTime; }

	void TimerOn();

	void TimerOff();

	bool SetTimerFlag(bool flag) { return isFlag = flag; } 

private:
	
	Input* input_ = nullptr;

	int mathTime = 0;

	int secondTime = 0;

	float wTime = 0.0f;

	bool isFlag = false;

	Sprite* texHandle[10] = {};

	

	//タイム 
	uint32_t textureHandleNumber_ = 0;
	Sprite* spriteNumber_[2] = {};
	int gameScore_ = 0;
	int number;
	int eachNumber[2] = {};
	int keta;
};