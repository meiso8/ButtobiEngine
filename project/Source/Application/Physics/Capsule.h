#pragma once
#include "Line.h"

/// @brief カプセル
struct Capsule {
	Segment segment;	// 中心線
	float radius;		// 半径
};