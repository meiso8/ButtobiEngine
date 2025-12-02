#include "CameraController.h"
#include"Camera.h"
#include<algorithm>
#include"Input.h"
#include"Random.h"
#include <math.h>
#include "MatsumotoObj/MY_Utility.h"

void CameraController::Update()
{

    UpdateShakeTimer();
	UpdateFocusTarget();
	UpdateEnemyLethal();

    camera_->UpdateMatrix();

}

void CameraController::Initialize()
{
    camera_->Initialize();
    camera_->translate_ = { 0.0f, 14.0f,-6.0f };
    camera_->rotate_ = { 1.2f,0.0f,0.0f };
    camera_->offset_ = { 0.0f,0.0f };
    isShakeStart_ = false;
    shakeFrame_ = 0;
    moveRange_ = 0.0f;

	isFocusTarget_ = false;
	focusTargetPos_ = nullptr;

	isEnemyLethal_ = false;

	cameraDefaultTransform_.translate = camera_->translate_;
	cameraDefaultTransform_.rotate = camera_->rotate_;
}

void CameraController::StartShake(const float& moveRange, const uint32_t& frame)
{
    if (isShakeStart_) {
        return;
    }

    isShakeStart_ = true;
    moveRange_ = moveRange;
    shakeFrame_ = frame;
}

void CameraController::FocusTarget(const Vector3* targetPos) {
	isFocusTarget_ = true;
	focusTargetPos_ = const_cast<Vector3*>(targetPos);
}

void CameraController::ResetFocusTarget() {
	isFocusTarget_ = false;
	focusTargetPos_ = nullptr;
}

void CameraController::UpdateFocusTarget() {
	if (!isFocusTarget_ || !focusTargetPos_) {
		camera_->rotate_ = MY_Utility::SimpleEaseIn(camera_->rotate_, cameraDefaultTransform_.rotate, 0.1f);
        camera_->fovAngleY_ = MY_Utility::SimpleEaseIn(camera_->fovAngleY_, 45.0f * 3.141592654f / 180.0f, 0.1f);
		return;
	}
	Vector3 targetRotate = MY_Utility::CalcLookAtRotation(camera_->translate_, *focusTargetPos_);
	camera_->rotate_ = MY_Utility::SimpleEaseIn(camera_->rotate_, targetRotate, 0.1f);
	camera_->fovAngleY_ = MY_Utility::SimpleEaseIn(camera_->fovAngleY_, 0.3f, 0.3f);
}

void CameraController::StartEnemyLethal() {
	isEnemyLethal_ = true;
}

void CameraController::UpdateEnemyLethal() {
	if (!isEnemyLethal_) {
		return;
	}
    camera_->rotate_.x = MY_Utility::SimpleEaseIn(camera_->rotate_.x, -0.2f, 0.1f);
	camera_->translate_.y = MY_Utility::SimpleEaseIn(camera_->translate_.y, 5.0f, 0.1f);
    camera_->translate_.z = MY_Utility::SimpleEaseIn(camera_->translate_.z, -10.0f, 0.1f);
}

void CameraController::EndEnemyLethal() {
	isEnemyLethal_ = false;
}

void CameraController::UpdateShakeTimer()
{
    if (shakeFrame_ == 0) {
        camera_->offset_ = { 0.0f,0.0f };
        return;
    }

    if (shakeFrame_ > 0) {
        shakeFrame_--;
    }
   
    if (shakeFrame_ % 6 == 0) {
        if (moveRange_ > 0.0f) {
            moveRange_--;
        }
    }

    Random::SetMinMax(-moveRange_, moveRange_);
    camera_->offset_.x = Random::Get();
    camera_->offset_.y = Random::Get();


}
