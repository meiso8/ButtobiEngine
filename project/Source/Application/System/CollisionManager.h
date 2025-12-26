#pragma once
#include <list>

class Collider;

/// @brief 衝突マネージャ
class CollisionManager {
public:
    /// @brief コライダーを追加する
    /// @param collider コライダー
    void AddCollider(Collider* collider) { colliders_.emplace_back(collider); }

    template <typename Container>
    void AddColliders(const Container& colliders) { 
        for (auto& collider : colliders) { 
            colliders_.emplace_back(collider);
        } }


    /// @brief コライダーをクリアする
    void ClearColliders() { colliders_.clear(); }

    /// @brief 衝突判定と応答
    void CheckAllCollisions();

private:
    std::list<Collider*> colliders_; // コライダーのリスト

    void  CheckCollisionPair(Collider* a, Collider* b);

    /// @brief コライダー2つの衝突判定と応答
    /// @param colliderA コライダーA
    /// @param colliderB コライダーB
    /// @param score スコアポインタ
    void CheckCollisionSpherePair(Collider* colliderA, Collider* colliderB);

    /// @brief コライダー2つの衝突判定と応答
/// @param colliderA コライダーA
/// @param colliderB コライダーB
/// @param score スコアポインタ
    void CheckCollisionAABBPair(Collider* colliderA, Collider* colliderB);

    /// @brief コライダー2つの衝突判定と応答
/// @param colliderA コライダーA
/// @param colliderB コライダーB
/// @param score スコアポインタ
    void CheckCollisionSphereAABBPair(Collider* colliderA, Collider* colliderB);
};