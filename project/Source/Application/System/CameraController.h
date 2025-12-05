#pragma once
class Camera;
#include<cstdint>
#include "Math/Vector/Vector3.h"
#include "Engine/Object3d/Object3d.h"
#include"Shake.h"
class CameraController
{
public:
    CameraController(Camera& camera) :camera_(&camera) {};
    void Update();
    void Initialize();
    void StartShake(const float& moveRange, const uint32_t& frame);
    void UpdateShakeTimer();

    void FocusTarget(const Vector3* targetPos);
    void ResetFocusTarget();
    void UpdateFocusTarget();

    void StartEnemyLethal();
    void UpdateEnemyLethal();
    void EndEnemyLethal();

private:
	Transform cameraDefaultTransform_;

    Camera* camera_ = nullptr;
    Shake shake_;

	bool isFocusTarget_ = false;
	Vector3* focusTargetPos_ = nullptr;

	bool isEnemyLethal_ = false;
};
