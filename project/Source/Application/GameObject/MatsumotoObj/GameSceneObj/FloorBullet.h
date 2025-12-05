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
enum class FloorType;

class FloorBullet :public Collider
{
public:
    FloorBullet();
    ~FloorBullet();
    void OnCollision(Collider* collider)override;
    Vector3 GetWorldPosition() const;
    void Initialize();
    void InitFlagAndPosAndTimer();
    void Update();
    void Draw(Camera& camera);
    void Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size,FloorType type);
	void SetHoming(Object3d* target, float stayTime);
    bool isActive_ = false;
    Object3d body_;
    float size_;
    void Move();
	void HomingMove();
    bool isHit_ = false;
    bool isHoming_ = false;

private:
    FloorType floorType_;
    FloorType reqestFloorType_;

    std::unordered_map<FloorType, Model*> models_;
    std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;
    Vector3 moveDir_;
    float moveSpeed_;
    float lifeTimer_;
    float lifeDuration_;
	Vector3 velocity_;

    float homingStayTime_;
	Object3d* homingTarget_ = nullptr;
};