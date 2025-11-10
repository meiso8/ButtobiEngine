#pragma once

#include"WorldTransform.h"
#include"Object3d.h"

class Model;
class Camera;
enum LightMode;

class Player
{
public:
    Player();
    void Init();
    void Draw(Camera& camera, const LightMode& lightType);
    void Update();
    void Move();
    Vector3& GetForward();
    void LookBack();
    void MouseLook();
    Matrix4x4 GetWorldMatrix() {
        return object3d_.worldTransform_.matWorld_;
    };
    float endRotateY_ = 0.0f;
    float startRotateY = 0.0f;
    float time_ = 1.0f;
    bool isEnd_ = false;

    bool isLookBack_ = false;
private:
    Vector3 velocity_;
    float kSpeed_;
    float walkingTheta_ = 0.0f;
    float soundTimer_ = 0.0f;
    Model*model_;
    Object3d object3d_;
};

