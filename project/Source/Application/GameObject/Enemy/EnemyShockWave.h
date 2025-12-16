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
    void Draw(Camera& camera);
    void Shot(const Vector3& startPos, const Vector3& endPos, const AABBType& aabbType);
    bool isActive_ = false;
    Object3d body_;
    std::array<AABB, kMaxAABB> localAABBs_;
    AABBType aabbType_ = kHorizontal;
    bool isEmit_ = false;
private:
    float lifeTimer_;
    const float kMoveTime_ = 5.0f;
    const float kAabbWidth_ = 0.4f;
    Vector3 endPos_ = { 0.0f };
    //std::array<std::unique_ptr<CubeMesh>,kMaxAABB>cubeMesh_;
    Vector3 moveDir_;
    float speed_;

    float lifeDuration_;
    bool isSound_ = false;

};
