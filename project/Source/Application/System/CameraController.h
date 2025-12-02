#pragma once
class Camera;
#include<cstdint>
#include "Math/Vector/Vector3.h"
#include "Engine/Object3d/Object3d.h"

class CameraController
{
public:
    CameraController(Camera& camera) :camera_(&camera) {};
    void Update();
    void Initialize();
    void StartShake(const float& moveRange, const uint32_t& frame);
    void SetShakeFalse() { isShakeStart_ = false; }
    void UpdateShakeTimer();

    void FocusTarget(const Vector3* targetPos);
    void ResetFocusTarget();
    void UpdateFocusTarget();

    void StartEnemyLethal();
    void UpdateEnemyLethal();
    void EndEnemyLethal();

private:
	Transform cameraDefaultTransform_;

    uint32_t  shakeFrame_ = 0;
    float moveRange_ = 0.0f;
    Camera* camera_ = nullptr;
    bool isShakeStart_ = false;

	bool isFocusTarget_ = false;
	Vector3* focusTargetPos_ = nullptr;

	bool isEnemyLethal_ = false;
};
