#include "timer.h"



// 指定された時間が経過するまで待機し、その後フラグをtrueに設定し、一定時間後にフラグをfalseに戻す関数
void timer::Timer(int seconds, bool* flag) {
	time_t start_time = time(NULL); // 開始時刻を取得
	time_t current_time;

	// 指定された時間が経過するまで待機
	do {
		current_time = time(NULL); // 現在時刻を取得
	} while ((current_time - start_time) < seconds);

	// 指定時間が経過したのでフラグをtrueに設定
	*flag = true;

	// 一定時間後にフラグをfalseに戻す
	Sleep(1*500); // 例えば、ここでは5秒後にフラグをfalseに戻す
	*flag = false;
}
