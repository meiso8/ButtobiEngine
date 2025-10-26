#define NOMINMAX // std::maxの置き換えが行われないため

#include "CameraController.h"
#include "Easing.h"
#include "Player.h"
#include <algorithm> // std::max, std::min を使用するために必要
#include"Camera/Camera.h"
#include<cmath>
#include"Window.h"
#include<numbers>

void CameraController::Initialize(Camera* camera) {
    camera_ = camera;
    camera_->Initialize(static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()));
    zoomInTimer_ = 0;
    zoomEnd_ = false;
}

void CameraController::Update() {
    const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

    // プレイヤー位置
    Vector3 playerPos = targetWorldTransform.translate_;
    Vector3 forward = { std::sin(targetWorldTransform.rotate_.y), 0.0f, std::cos(targetWorldTransform.rotate_.y) };
    forward = Normalize(forward);

    // forward の逆方向に一定距離離した位置にカメラを置く
    float distance = targetOffset_.z; // 背後距離
    Vector3 behindPos = playerPos - forward * distance;
    // 見下ろし高さ
    float height = 5.0f;
    behindPos.y += height;

    camera_->translate_ = Easing::EaseOutQuad(camera_->translate_, behindPos, 0.5f);
    camera_->rotate_.x = 0.30f;
    camera_->rotate_.y = Easing::EaseInQuad(camera_->rotate_.y, targetWorldTransform.rotate_.y, 0.75f);
}

void CameraController::ZoomIn()
{
    if (zoomInTimer_ < 60) {
        zoomInTimer_++;
        const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

        // プレイヤー位置
        Vector3 playerPos = targetWorldTransform.translate_;
        Vector3 forward = target_->GetForward();

        // forward の逆方向に一定距離離した位置にカメラを置く
        float distance = 5.0f; // 背後距離
        Vector3 forwardPos = playerPos + forward * distance;
        // 見下ろし高さ
        float height = 1.0f;
        forwardPos.y += height;

        camera_->translate_ = Easing::EaseOutQuad(camera_->translate_, forwardPos, 0.5f);
        camera_->rotate_.x = 0.30f;
        float rotate = std::atan2f(forward.z, forward.x);
        rotate += std::numbers::pi_v<float> / 2.0f;
        camera_->rotate_.y = Easing::EaseInQuad(camera_->rotate_.y, rotate, 0.75f);

    } else {
        zoomInTimer_ = 60;
        zoomEnd_ = true;
    }

}


void CameraController::Reset() {

    // 追従対象のワールドトランスフォーム
    const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
    // 追従対象とオフセットからカメラ座標を計算
    camera_->translate_ = targetWorldTransform.translate_ + targetOffset_;
};

