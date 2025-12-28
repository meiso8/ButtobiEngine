#pragma once

#include"WorldTransform.h"
#include"Object3d.h"
#include"Circle.h"
#include"CharacterState.h"
#include"AABB.h"
#include"Line.h"
#include"Collider.h"
#include"RaySprite.h"

class Model;
class Camera;
enum LightMode;
class CircleMesh;
class CubeMesh;
class Sprite;

class Player :public Collider
{
public:
    void OnCollision(Collider* collider)override;

    /// @brief ワールド座標を取得する
    /// @return ワールド座標
    Vector3 GetWorldPosition() const  override;
    Player();
    void Init();
 
    void Draw(Camera& camera, const LightMode& lightType);
    void Update();
    
    
    void UpdateRay();
    void Move();
    void Zoom();
    void PointLightSwitch();
    void LookBack();
    void MouseLook();

    Vector3& GetForward();


    Matrix4x4& GetEyeMatrix() {
        return eyePos_.worldTransform_.matWorld_;
    };
    WorldTransform& GetEyeWorldTransform() {
        return eyePos_.worldTransform_;
    }
    Vector3& GetBodyPos() {
        return bodyPos_.worldTransform_.translate_;
    };

    Circle& GetCircle() {
        return circle_;
    };
    HPs* GetHpsPtr() { return &characterState_.hps; }
    void OnCollision(const Circle& circle);
    void ResolveCollision(const AABB& wallAABB, const AABB& playerAABB);
    void OnCollisionWall();

    float cameraSpeed_ = 1.0f;
    bool isPressSpace_ = false;

    float zoomTimer_ = 1.0f;
    bool isZoom_ = false;

    std::unique_ptr<RaySprite> raySprite_ = nullptr;
private:
    float hitTimer_ = 0.0f;
    float endRotateY_ = 0.0f;
    float startRotateY = 0.0f;
    float lookBackTime_ = 1.0f;
    bool isLookBackEnd_ = false;

    bool isLookBack_ = false;

    bool isWallHit = false;


    //AABB
    AABB localAabb_;
    Circle circle_;
    Vector3 velocity_;
    float kSpeed_;
    float walkingTheta_ = 0.0f;
    float soundTimer_ = 0.0f;
    Model* model_;

    Object3d bodyPos_;
    Object3d eyePos_;

    float cameraRotateY_ = 0.0f;
    float cameraRotateX_ = 0.0f;

    CharacterState characterState_;


    void OnCollisionEnemy();
};

