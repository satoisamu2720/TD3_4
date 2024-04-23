#pragma once
#include "ImGuiManager.h"
#include "Sprite.h"
#include "MT.h"
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

	void SetStartTimer(int time);

	int GetTimeMath() { return mathTime; }
	int  GetTimeSecond() { return secondTime; }

	int GetStartTime() { return startTimer; }

	void TimerOn();

	void TimerOff();

	bool SetTimerFlag(bool flag) { return isFlag = flag; } 

	bool SetStartTimerFlag(bool flag) { return startIsFlag = flag; }

private:
	
	Input* input_ = nullptr;

	int mathTime = 0;

	int secondTime = 0;

	float wTime = 0.0f;

	bool isFlag = false;



	int startTimer = 3;

	float wStartTime = 0.0f;

	bool startIsFlag = false;




};