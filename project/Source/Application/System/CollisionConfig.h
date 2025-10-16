#pragma once
#include <cstdint>

namespace {
	// プレイヤー陣営
	constexpr uint32_t kCollisionAttributePlayer = 0b1;

	// 敵陣営
	constexpr uint32_t kCollisionAttributeEnemy = 0b1 << 1;
}