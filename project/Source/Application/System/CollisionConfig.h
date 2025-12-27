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
    constexpr uint64_t kCollisionEnemyBullet = 0b1 << 3;
    // 敵陣営
    constexpr uint32_t kCollisionMedjed = 0b1 << 4;
    // 壁
    constexpr uint64_t kCollisionWall = 0b1 << 5;
    // ロッカー
    constexpr uint64_t kCollisionLocker = 0b1 << 6;
    // アイテム
    constexpr uint64_t kCollisionItem = 0b1 << 7;
    // 何の陣営にも属さない
    constexpr uint32_t kCollisionNone = 0b0;

}
