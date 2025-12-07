#include "MY_Utility.h"
#include <math.h>
#include <random>

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

Vector2 MY_Utility::SimpleEaseIn(const Vector2& from, const Vector2& to, float transitionSpeed) {
	Vector2 value = from;
	value.x = SimpleEaseIn(from.x, to.x, transitionSpeed);
	value.y = SimpleEaseIn(from.y, to.y, transitionSpeed);
	return value;
}

Vector2 MY_Utility::SimpleEaseOut(const Vector2& from, const Vector2& to, float transitionSpeed) {
	Vector2 value = from;
	value.x = SimpleEaseOut(from.x, to.x, transitionSpeed);
	value.y = SimpleEaseOut(from.y, to.y, transitionSpeed);
	return value;
}

Vector3 MY_Utility::SimpleEaseIn(const Vector3& from, const Vector3& to, float transitionSpeed) {
	Vector3 value = from;
	value.x = SimpleEaseIn(from.x, to.x, transitionSpeed);
	value.y = SimpleEaseIn(from.y, to.y, transitionSpeed);
	value.z = SimpleEaseIn(from.z, to.z, transitionSpeed);
	return value;
}

Vector3 MY_Utility::SimpleEaseOut(const Vector3& from, const Vector3& to, float transitionSpeed) {
	Vector3 value = from;
	value.x = SimpleEaseOut(from.x, to.x, transitionSpeed);
	value.y = SimpleEaseOut(from.y, to.y, transitionSpeed);
	value.z = SimpleEaseOut(from.z, to.z, transitionSpeed);
	return value;
}

Vector4 MY_Utility::SimpleEaseIn(const Vector4& from, const Vector4& to, float transitionSpeed) {
	Vector4 value = from;
	value.x = SimpleEaseIn(from.x, to.x, transitionSpeed);
	value.y = SimpleEaseIn(from.y, to.y, transitionSpeed);
	value.z = SimpleEaseIn(from.z, to.z, transitionSpeed);
	value.w = SimpleEaseIn(from.w, to.w, transitionSpeed);
	return value;
}

Vector4 MY_Utility::SimpleEaseOut(const Vector4& from, const Vector4& to, float transitionSpeed) {
	Vector4 value = from;
	value.x = SimpleEaseOut(from.x, to.x, transitionSpeed);
	value.y = SimpleEaseOut(from.y, to.y, transitionSpeed);
	value.z = SimpleEaseOut(from.z, to.z, transitionSpeed);
	value.w = SimpleEaseOut(from.w, to.w, transitionSpeed);
	return value;
}

Vector3 MY_Utility::CalcLookAtRotation(const Vector3& from, const Vector3& to) {
	Vector3 result;
	Vector3 direction = to - from;
	float distanceXZ = sqrtf(direction.x * direction.x + direction.z * direction.z);
	result.x = -atan2f(direction.y, distanceXZ);
	result.y = atan2f(direction.x, direction.z);
	result.z = 0.0f;
	return result;
}

bool MY_Utility::IsActiveForTimerSwitch(float& timer, float interval) {
	return  (fmod(timer, interval * 2) < interval);
}

float MY_Utility::Length(const Vector2& vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

bool MY_Utility::RandomBool(float trueProbability)
{
	float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return randomValue < trueProbability;
}
