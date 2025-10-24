#define NOMINMAX // std::maxの置き換えが行われないため

#include "CameraController.h"
#include "Easing.h"
#include "Player.h"
#include <algorithm> // std::max, std::min を使用するために必要
#include"Camera/Camera.h"
#include<cmath>
#include"Window.h"

void CameraController::Initialize(Camera* camera) {
    camera_ = camera;
    camera_->Initialize(static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()));
}

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// プレイヤー位置
	Vector3 playerPos = targetWorldTransform.translate_;
	Vector3 forward = {std::sin(targetWorldTransform.rotate_.y), 0.0f, std::cos(targetWorldTransform.rotate_.y)};
	forward = Normalize(forward);

	// forward の逆方向に一定距離離した位置にカメラを置く
	float distance = targetOffset_.z; // 背後距離
	Vector3 behindPos = playerPos - forward * distance;
	// 見下ろし高さ
	float height = 5.0f;
	behindPos.y += height;

	camera_->translate_ = Easing::EaseOutQuad(camera_->translate_, behindPos, 0.25f);
	camera_->rotate_.x = 0.30f;
	camera_->rotate_.y = Easing::EaseInQuad(camera_->rotate_.y, targetWorldTransform.rotate_.y, 0.25f);
}


void CameraController::Reset() {

    // 追従対象のワールドトランスフォーム
    const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
    // 追従対象とオフセットからカメラ座標を計算
	camera_->translate_ = targetWorldTransform.translate_ + targetOffset_;
};

