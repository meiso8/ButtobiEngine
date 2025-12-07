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
	UpdateOverheadView();

    camera_->UpdateMatrix();
}

void CameraController::Initialize()
{
    camera_->Initialize();
    camera_->translate_ = { 0.0f, 17.0f,-7.4f };
    camera_->rotate_ = { 1.2f,0.0f,0.0f };
    camera_->offset_ = { 0.0f,0.0f };
    shake_.Reset();

	isFocusTarget_ = false;
	focusTargetPos_ = nullptr;
	focusFov_ = 0.35f;

	isEnemyLethal_ = false;

	cameraDefaultTransform_.translate = camera_->translate_;
	cameraDefaultTransform_.rotate = camera_->rotate_;
}

void CameraController::StartShake(const float& moveRange, const uint32_t& frame)
{
    shake_.Start(moveRange, frame);
}
void CameraController::FocusTarget(const Vector3* targetPos, float fov)
{
	isFocusTarget_ = true;
	focusTargetPos_ = const_cast<Vector3*>(targetPos);
	focusFov_ = fov;

	isOverheadView_ = false;
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
	camera_->fovAngleY_ = MY_Utility::SimpleEaseIn(camera_->fovAngleY_, focusFov_, 0.3f);
}

void CameraController::StartOverheadView()
{
	isOverheadView_ = true;
}

void CameraController::UpdateOverheadView()
{
	if (!isOverheadView_) {
		camera_->rotate_ = MY_Utility::SimpleEaseIn(camera_->rotate_, cameraDefaultTransform_.rotate, 0.1f);
		camera_->translate_ = MY_Utility::SimpleEaseIn(camera_->translate_, cameraDefaultTransform_.translate, 0.1f);
		return;
	}
	camera_->rotate_.x = MY_Utility::SimpleEaseIn(camera_->rotate_.x, 1.68f, 0.1f);
	camera_->translate_.y = MY_Utility::SimpleEaseIn(camera_->translate_.y, 30.0f, 0.1f);
	camera_->translate_.z = MY_Utility::SimpleEaseIn(camera_->translate_.z, 0.0f, 0.1f);
}

void CameraController::EndOverheadView()
{
	isOverheadView_ = false;
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
    shake_.Update();
	camera_->offset_ = shake_.GetOffset();
}
