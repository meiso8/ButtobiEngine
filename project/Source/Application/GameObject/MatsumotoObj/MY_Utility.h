#pragma once
#include "Engine/Math/Vector/Vector2.h"
#include "Engine/Math/Vector/Vector3.h"
#include "Engine/Math/Vector/Vector4.h"

namespace MY_Utility {
	float SimpleEaseIn(float from, float to, float transitionSpeed);
	float SimpleEaseOut(float from, float to, float transitionSpeed);

	Vector2 SimpleEaseIn(const Vector2& from, const Vector2& to, float transitionSpeed);
	Vector2 SimpleEaseOut(const Vector2& from, const Vector2& to, float transitionSpeed);

	Vector3 SimpleEaseIn(const Vector3& from, const Vector3& to, float transitionSpeed);
	Vector3 SimpleEaseOut(const Vector3& from, const Vector3& to, float transitionSpeed);

	Vector4 SimpleEaseIn(const Vector4& from, const Vector4& to, float transitionSpeed);
	Vector4 SimpleEaseOut(const Vector4& from, const Vector4& to, float transitionSpeed);

	Vector3 CalcLookAtRotation(const Vector3& from, const Vector3& to);

	bool IsActiveForTimerSwitch(float& timer, float interval);

	float Length(const Vector2& vec);
} 