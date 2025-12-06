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

class EnemyBomb :public Collider
{
public:
    EnemyBomb();
    ~EnemyBomb();
    void OnCollision(Collider* collider)override;
    Vector3 GetWorldPosition() const;
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void Shot(const Vector3& startPos, const Vector3& endPos, const float& size);
    bool isActive_ = false;
    Object3d body_;
    float size_;
	bool isGroundHit_ = false;
private:
    int explosionFrame_ = 0;
    float kRadius_ = {0.5f};
    Vector3 endPos_ = { 0.0f };
    Model* model_ = nullptr;
    float speed_;
    float lifeTimer_;
    float lifeDuration_;
	float lifeDelay_;
};