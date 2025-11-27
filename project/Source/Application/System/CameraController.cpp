#include "CameraController.h"
#include"Camera.h"
#include<algorithm>
#include"Input.h"
#include"Random.h"
#include <math.h>

void CameraController::Update()
{

    if (isShake_) {

        shakeFrame_ --;

        if (shakeFrame_ < 0) {
            isShake_ = false;
            camera_->offset_ = { 0.0f,0.0f };
            shakeFrame_ = 0;
        }

        if (moveRange_ > 0.0f) {
            // スペースキーを押したとき
            if (shakeFrame_ % 6 == 0) {
                moveRange_--;
            }
        } else {
            Random::SetMinMax(moveRange_, moveRange_);
            camera_->offset_.x = Random::Get();
            camera_->offset_.y = Random::Get();
        }

    }

    camera_->UpdateMatrix();

}

void CameraController::Initialize()
{
    camera_->Initialize();
    camera_->translate_ = { 0.0f, 14.0f,-6.0f };
    camera_->rotate_ = { 1.2f,0.0f,0.0f };
    camera_->UpdateMatrix();
}

void CameraController::StartShake(const float& moveRange, const uint32_t& frame)
{
    if (isShake_) {
        return;
    }

    isShake_ = true;
    moveRange_ = moveRange;
    shakeFrame_ = frame;
}
