#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include <functional>
#include <unordered_map>
#include"Collider.h"

class Model;
class Camera;
enum LightMode;
class CubeMesh;

class EnemyBullet :public Collider
{
public:
    EnemyBullet();
    ~EnemyBullet();
    void OnCollision(Collider* collider)override;
    Vector3 GetWorldPosition() const;
    void Initialize();
    void Update();
    void Draw(Camera& camera, const LightMode& lightType);
    void Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size);
    bool isActive_ = false;
    Object3d body_;
    float size_;
private:
    Model* model_ = nullptr;
    Vector3 moveDir_;
    float moveSpeed_;
    float lifeTimer_;
    float lifeDuration_;
};