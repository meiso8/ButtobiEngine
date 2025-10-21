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

    /// @brief スコアポインタを設定する
    /// @param score スコアポインタ
    void SetScorePointer(uint32_t* score) { score_ = score; }

    /// @brief スコアポインタを設定する
/// @param score スコアポインタ
    void SetComboPointer(uint32_t* combo) { combo_ = combo; }
    void SetComboTimerPtr(float* comBoTimer) { comboTimer_ = comBoTimer ;}
private:
    std::list<Collider*> colliders_; // コライダーのリスト
    uint32_t* score_ = nullptr;      // スコアポインタ
    uint32_t* combo_ = nullptr;//コンポのポインタ
    float* comboTimer_ = nullptr;//コンボタイマーのポインタ
    /// @brief コライダー2つの衝突判定と応答
    /// @param colliderA コライダーA
    /// @param colliderB コライダーB
    /// @param score スコアポインタ
    void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};