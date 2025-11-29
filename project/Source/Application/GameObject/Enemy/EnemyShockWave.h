#pragma once

#include"WorldTransform.h"
#include"Object3d.h"
#include <functional>
#include <unordered_map>
#include"Collider.h"
#include<array>
class Model;
class Camera;
enum LightMode;
class CubeMesh;

class EnemyShockWave :public Collider
{
public:
    enum AABBType {
        kHorizontal,
        kVertical,
        kMaxAABB,
    };
    EnemyShockWave();
    ~EnemyShockWave();
    void OnCollision(Collider* collider)override;
    Vector3 GetWorldPosition() const;
    void Initialize();
    void Update();
    void Draw(Camera& camera, const LightMode& lightType);
    void Shot(const Vector3& startPos, const Vector3& endPos, const AABBType& aabbType);
    bool isActive_ = false;
    Object3d body_;
private:
    const float kMoveTime_ = 5.5f;
    const float kAabbWidth_ = 0.4f;

    std::array<AABB, kMaxAABB> localAABBs_;

    Vector3 endPos_ = { 0.0f };
    std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;
    Vector3 moveDir_;
    float speed_;
    float lifeTimer_;
    float lifeDuration_;

};
