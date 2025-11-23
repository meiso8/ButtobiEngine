#pragma once
#include <cstdint>

namespace {
// プレイヤー陣営
constexpr uint32_t kCollisionPlayer = 0b1;
// プレイヤー陣営
constexpr uint32_t kCollisionPlayerBullet = 0b1 << 1;
// 敵陣営
constexpr uint32_t kCollisionEnemy = 0b1 << 2;
// 敵陣営
constexpr uint32_t kCollisionEnemyBullet = 0b1 << 3;
// 敵陣営
constexpr uint32_t kCollisionEnemyBomb = 0b1 << 4;

// 床
constexpr uint32_t kCollisionFloor= 0b1 << 5;

}
