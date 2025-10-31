#pragma once
#include <list>

class Collider;

/// @brief 衝突マネージャ
class CollisionManager {
public:
    /// @brief コライダーを追加する
    /// @param collider コライダー
    void AddCollider(Collider* collider) { colliders_.emplace_back(collider); }

    /// @brief コライダーをクリアする
    void ClearColliders() { colliders_.clear(); }

    /// @brief 衝突判定と応答
    void CheckAllCollisions();

private:
    static inline constexpr float deltaTime = 1.0f / 60.0f; // デルタタイム
    std::list<Collider*> colliders_; // コライダーのリスト
    uint32_t* score_ = nullptr;      // スコアポインタ
    uint32_t* combo_ = nullptr;//コンポのポインタ
    float* comboTimer_ = nullptr;//コンボタイマーのポインタ
	bool* isScoreUp_ = nullptr;//スコア加算するかのポインタ
	bool* isComboSpakle_ = nullptr; // コンボの花火出すかのポインタ
	uint32_t* juiceMeter_ = nullptr;//ジュースメーターのポインタ
    /// @brief コライダー2つの衝突判定と応答
    /// @param colliderA コライダーA
    /// @param colliderB コライダーB
    /// @param score スコアポインタ
    void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};