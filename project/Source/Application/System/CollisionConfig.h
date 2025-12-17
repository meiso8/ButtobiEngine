#pragma once
#include <cstdint>

namespace {
// プレイヤー陣営
constexpr uint32_t kCollisionPlayer = 0b1;
// プレイヤー陣営
constexpr uint32_t kCollisionPlayerBullet = 0b1 << 1;
// プレイヤー陣営
constexpr uint32_t kCollisionPlayerHealItem = 0b1 << 8;
// 敵陣営
constexpr uint32_t kCollisionEnemy = 0b1 << 2;
// 敵陣営
constexpr uint32_t kCollisionEnemyBullet = 0b1 << 3;
// 敵陣営
constexpr uint32_t kCollisionEnemyBomb = 0b1 << 4;
// 敵陣営
constexpr uint32_t kCollisionEnemyWave = 0b1 << 5;
// 床
constexpr uint32_t kCollisionFloor= 0b1 << 6;
// 強化床
constexpr uint32_t kCollisionStrongFloor = 0b1 << 9;

// 家
constexpr uint32_t kCollisionHouse = 0b1 << 7;
// 何の陣営にも属さない
constexpr uint32_t kCollisionNone = 0b0;

}
