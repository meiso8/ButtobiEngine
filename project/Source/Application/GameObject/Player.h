#pragma once

#include"WorldTransform.h"
#include"Object3d.h"
#include"Circle.h"
#include"CharacterState.h"
#include"AABB.h"


class Model;
class Camera;
enum LightMode;
class CircleMesh;
class CubeMesh;
class Sprite;
class Player
{
public:
    Player();
    void Init();
    void UpdateRay();
    void Draw(Camera& camera, const LightMode& lightType);
    void Update();
    void Move();
    void Zoom();
    void PointLightSwitch();
    Vector3& GetForward();
    void LookBack();
    void MouseLook();
    Matrix4x4& GetEyeMatrix() {
        return eyePos_.worldTransform_.matWorld_;
    };
    WorldTransform& GetEyeWorldTransform() {
        return eyePos_.worldTransform_;
    }
    Vector3& GetBodyPos() {
        return bodyPos_.worldTransform_.translate_;
    };

    AABB GetWorldAABB();
    Circle& GetCircle() {
        return circle_;
    };
    HPs* GetHpsPtr() {return &characterState_.hps; }
    void OnCollision(const Circle& circle);
    void ResolveCollision(const AABB& wallAABB, const AABB& playerAABB);
    void OnCollisionWall(const AABB& aabb);
    void OnCollisionEnemy();

    float cameraSpeed_ = 1.0f;
    bool isPressSpace_ = false;
    float zoomTimer_ = 1.0f;
    bool isZoom_ = false;
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
    Object3d rayEndPos_;
   
    float cameraRotateY_ = 0.0f;
    float cameraRotateX_ = 0.0f;

    CharacterState characterState_;



    std::unique_ptr<Sprite>centerSprite_ = nullptr;

};

