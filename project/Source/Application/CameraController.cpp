#define NOMINMAX // std::maxの置き換えが行われないため

#include "CameraController.h"
#include "Lerp.h"
#include "Player.h"
#include <algorithm> // std::max, std::min を使用するために必要
#include"Camera/Camera.h"
#include<cmath>
#include"Normalize.h"

void CameraController::Initialize(Camera* camera) {
	camera_ = camera;
	camera_->Initialize(1280.0f,720.0f);
}

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// プレイヤー位置
	Vector3 playerPos = targetWorldTransform.translate_;

	
	Vector3 forward = {std::sin(targetWorldTransform.rotate_.y), 0.0f, std::cos(targetWorldTransform.rotate_.y)};
	forward = Normalize(forward);

	// forward の逆方向に一定距離離した位置にカメラを置く
	float distance = targetOffset_.z; // 背後距離
	float height = 5.0f;    // 見下ろし高さ
	Vector3 behindPos = {playerPos.x - forward.x * distance, playerPos.y - forward.y * distance, playerPos.z - forward.z * distance};
	behindPos.y += height;

	// カメラの目標座標
	targetPos_ = behindPos;

	// ゆったり追従
	camera_->translate_ = Lerp(camera_->translate_, targetPos_, kInterpolationRate);

	// 移送範囲制限
	camera_->translate_.y = std::max(camera_->translate_.y, targetWorldTransform.translate_.y + margin.bottom);
	camera_->translate_.y = std::min(camera_->translate_.y, targetWorldTransform.translate_.y + margin.top);
	
	camera_->rotate_.x = 0.3f;
	camera_->rotate_.y = targetWorldTransform.rotate_.y;

}


void CameraController::Reset() {

	// 追従対象のワールドトランスフォーム
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラ座標を計算
	camera_->translate_ = targetWorldTransform.translate_ + targetOffset_;
};

