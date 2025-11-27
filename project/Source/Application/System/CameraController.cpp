#include "CameraController.h"
#include"Camera.h"
#include<algorithm>
#include"Input.h"
#include"Random.h"
#include <math.h>

void CameraController::Update()
{

    UpdateShakeTimer();

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
