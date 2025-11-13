#include "MY_Utility.h"
#include <math.h>

float MY_Utility::SimpleEaseIn(float from, float to, float transitionSpeed) {
	float value = from;
	value += (to - value) * transitionSpeed;
	if (fabsf(value - to) <= 0.01f) {
		return to;
	}
	return value;
}

float MY_Utility::SimpleEaseOut(float from, float to, float transitionSpeed) {
	float value = from;
	value += (to - value) * (1.0f - transitionSpeed);
	if (fabsf(value - to) <= 0.01f) {
		return to;
	}
	return value;
}
