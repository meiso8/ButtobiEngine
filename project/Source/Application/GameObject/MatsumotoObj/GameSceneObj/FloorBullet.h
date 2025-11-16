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

class FloorBullet :public Collider
{
public:
    FloorBullet();
    ~FloorBullet();
    void OnCollision()override;
    Vector3 GetWorldPosition() const;
    void Initialize();
    void Update();
    void Draw(Camera& camera, const LightMode& lightType);
    void Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size);
    bool isActive_ = false;
    Object3d body_;
    float size_;
private:
    std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;
    Vector3 moveDir_;
    float moveSpeed_;
    float lifeTimer_;
    float lifeDuration_;
};