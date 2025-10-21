#pragma once
#include "Vector3.h"
#include <array>
#include <memory>
#include <string>
#include <numbers>

/// @brief 球
struct Sphere {
	Vector3 center;	// 中心座標
	float radius;	// 半径
};
