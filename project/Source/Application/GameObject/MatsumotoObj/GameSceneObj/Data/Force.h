#pragma once
#include "Engine/Math/Vector/Vector3.h"

class Force {
public:
	void Update() {
		velocity += acceleration * 0.016f;
		velocity.x *= (1.0f - friction);
		velocity.y *= (1.0f - friction);
		velocity.z *= (1.0f - friction);
		if (isGravity) {
			velocity.y -= gravityPower * 0.016f;
		}
	}

	Vector3 acceleration = { 0.0f,0.0f,0.0f };
	Vector3 velocity = { 0.0f,0.0f,0.0f };
	float friction = 0.02f;

	bool isGravity = true;
	float gravityPower = 0.98f;

	float timeScale = 1.0f;
};