#include "timer.h"
#include "TextureManager.h"

void Timer::Initialize() { 
	
	
}

void Timer::Update() {

	if (isFlag == true) {
		wTime -= 18;
	}

	if (isFlag == true) {

		if (mathTime == 0 && secondTime == 0) {
			isFlag = false;
			wTime = 1000;
		}
		if (wTime <= 0 && secondTime > 0) {
			secondTime -= 1;
			wTime = 1000;
		}
	}

	if (startIsFlag == true) {
		wStartTime -= 18;
	}

	if (startIsFlag == true) {

		if (wStartTime <= 0 && startTimer > 0) {
			startTimer -= 1;
			wStartTime = 1000;
		}
	}


#ifdef _DEBUG
	ImGui::Begin("Timer");

	if (isFlag == true) {
		ImGui::Text("isFlag = True");
	} else {
		ImGui::Text("isFlag = False");
	}
	ImGui::Checkbox("Timer Flag", &isFlag);

	ImGui::InputInt("%ds", & secondTime,1);

	ImGui::End();

	
#endif
}

void Timer::Draw() {

}

void Timer::SetTime(int math, int second) {

	if (isFlag == false) {

		mathTime = math;
		secondTime = second;
	}
}

void Timer::SetStartTimer(int time) {
	if (startIsFlag == false) {

		startTimer = time;
		
	}
}

void Timer::TimerOn() { isFlag = true; }

void Timer::TimerOff() { isFlag = false; }
