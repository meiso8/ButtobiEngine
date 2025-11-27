#pragma once
class Camera;
#include<cstdint>
class CameraController
{
public:
    CameraController(Camera& camera) :camera_(&camera) {};
    void Update();
    void Initialize();
    void StartShake(const float& moveRange, const uint32_t& frame);
    void UpdateShakeTimer();
    void SetShakeFalse() { isShakeStart_ = false; }
private:
    uint32_t  shakeFrame_ = 0;
    float moveRange_ = 0.0f;
    Camera* camera_ = nullptr;
    bool isShakeStart_ = false;
};
