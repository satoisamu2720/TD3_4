#include "Easings.h"
#include <math.h>

float easeInSine(float x) {

	float PI = 3.14f;
	return 1 - cosf((x * PI) / 2);
}
