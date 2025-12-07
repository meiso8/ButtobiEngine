#pragma once
#include"Collider.h"
class Camera;
class EnemyBeak :public Collider
{
public:

    EnemyBeak();
    void  Initialize();
    void Update();
    /// @brief 衝突時コールバック関数
    void OnCollision(Collider* collider)override;
    void Draw(Camera& camera);
    /// @brief ワールド座標を取得する
    /// @return ワールド座標
    Vector3 GetWorldPosition() const override;

    bool isFloorHit_ = false;

    WorldTransform beak_;

};

