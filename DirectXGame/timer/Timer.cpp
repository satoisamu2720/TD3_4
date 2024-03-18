#include "Timer.h"
#include "TextureManager.h"

void Timer::Initialize() {

	


}

void Timer::Update() {
	
	

	if (isFlag == true) {
		wTime -= 18;
	}

	if (isFlag == true) {

		if (MathTime == 0 && SecondTime == 0) {
			isFlag = false;
			wTime = 1000;
		}
		if (wTime <= 0 && SecondTime > 0) {
			SecondTime -= 1;
			wTime = 1000;
		}
		if (SecondTime <= 0 && MathTime > 0) {
			SecondTime = 59;
			MathTime -= 1;
		}
		
	}

	ImGui::Begin("Timer");

	if (isFlag == true) {
		ImGui::Text("isFlag = True");
	} else {
		ImGui::Text("isFlag = False");
	
	}
	

	ImGui::Text("%1.0fm : %1.0fs", MathTime, SecondTime);

	ImGui::End();


}

void Timer::Draw() {





}

void Timer::SetTime(float math, float second) {

	if (isFlag == false) {

		MathTime = math;
		SecondTime = second;
	}


}

void Timer::TimerOn() {

	isFlag = true;

}

void Timer::TimerOff() {

	isFlag = false;

}
