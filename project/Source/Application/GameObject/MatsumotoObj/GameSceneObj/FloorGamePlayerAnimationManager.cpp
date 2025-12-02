#include "FloorGamePlayerAnimationManager.h"
#include "FloorGamePlayer.h"
#include "FloorGameFloorManager.h"

#include "MatsumotoObj/MY_Utility.h"

namespace
{
	Vector3 headBasePos_ = { 0.0f,0.3f,0.0f };
	Vector3 rightArmBasePos_ = { 0.5f,0.0f,0.0f };
	Vector3 leftArmBasePos_ = { -0.5f,0.0f,0.0f };
	Vector3 rightLegBasePos = { 0.3f,-0.2f,-0.3f };
	Vector3 leftLegBasePos = { -0.3f,-0.2f,-0.3f };
}

FloorGamePlayerAnimationManager::FloorGamePlayerAnimationManager(FloorGamePlayer* player, FloorGameFloorManager* floorManager)
	: player_(player)
	, floorManager_(floorManager)
{
	animationStateAction_ = {
		{PlayerAnimationState::Idle, std::bind(&FloorGamePlayerAnimationManager::IdleAnimation, this)},
		{PlayerAnimationState::Walk, std::bind(&FloorGamePlayerAnimationManager::WalkAnimation, this)},
		{PlayerAnimationState::Stript, std::bind(&FloorGamePlayerAnimationManager::StriptAnimation, this)},
		{PlayerAnimationState::Shot, std::bind(&FloorGamePlayerAnimationManager::ShotAnimation, this)}
	};
}

FloorGamePlayerAnimationManager::~FloorGamePlayerAnimationManager() {
}

void FloorGamePlayerAnimationManager::Update() {
	player_->body_.worldTransform_.scale_.x = MY_Utility::SimpleEaseIn(player_->body_.worldTransform_.scale_.x, 1.0f, 0.1f);
	player_->body_.worldTransform_.scale_.y = MY_Utility::SimpleEaseIn(player_->body_.worldTransform_.scale_.y, 1.0f, 0.1f);
	player_->body_.worldTransform_.scale_.z = MY_Utility::SimpleEaseIn(player_->body_.worldTransform_.scale_.z, 1.0f, 0.1f);

	if (player_->GetDamageStruct().hps.hp <= 0)
	{
		return;
	}

	animationTimer_ += deltaTime_;
	animationStateAction_[player_->animationState_]();
}

void FloorGamePlayerAnimationManager::IdleAnimation() {
	float groundOffsetY = 0.25f;
	float animSpeed = 0.5f;

	// Y座標のイージング
	float targetBodyY = 0.5f + (groundOffsetY * sinf(animationTimer_ * 6.0f));
	player_->body_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(
		player_->body_.worldTransform_.translate_.y, targetBodyY, animSpeed);

	// 足の回転のイージング
	float targetRightLegRotX = 0.1f * sinf(animationTimer_ * 6.0f + 3.14f);
	float targetLeftLegRotX = 0.1f * sinf(animationTimer_ * 6.0f + 3.14f);
	player_->rightLegObject_.worldTransform_.rotate_.x = MY_Utility::SimpleEaseIn(
		player_->rightLegObject_.worldTransform_.rotate_.x, targetRightLegRotX, animSpeed);
	player_->leftLegObject_.worldTransform_.rotate_.x = MY_Utility::SimpleEaseIn(
		player_->leftLegObject_.worldTransform_.rotate_.x, targetLeftLegRotX, animSpeed);

	// 腕と足の位置（translate）を0.0fに戻す
	player_->rightLegObject_.worldTransform_.translate_ = MY_Utility::SimpleEaseIn(
		player_->rightLegObject_.worldTransform_.translate_, rightLegBasePos, animSpeed);

	player_->leftLegObject_.worldTransform_.translate_ = MY_Utility::SimpleEaseIn(
		player_->leftLegObject_.worldTransform_.translate_, leftLegBasePos, animSpeed);

	if (!(player_->isStriptting_ || player_->isReqestStript_)) {
		float targetBodyScaleY = 1.0f + (groundOffsetY * sinf(animationTimer_ * 6.0f));
		float targetBodyScaleX = 1.0f + (0.1f * sinf(animationTimer_ * 6.0f));
		player_->body_.worldTransform_.scale_.y = MY_Utility::SimpleEaseIn(
			player_->body_.worldTransform_.scale_.y, targetBodyScaleY, animSpeed);
		player_->body_.worldTransform_.scale_.x = MY_Utility::SimpleEaseIn(
			player_->body_.worldTransform_.scale_.x, targetBodyScaleX, animSpeed);

		player_->rightArmObject_.worldTransform_.translate_ = MY_Utility::SimpleEaseIn(
			player_->rightArmObject_.worldTransform_.translate_, rightArmBasePos_, animSpeed);

		player_->leftArmObject_.worldTransform_.translate_ = MY_Utility::SimpleEaseIn(
			player_->leftArmObject_.worldTransform_.translate_, leftArmBasePos_, animSpeed);
	} else {
		player_->rightArmObject_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(
			player_->rightArmObject_.worldTransform_.translate_.y, 0.8f, animSpeed);
		player_->leftArmObject_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(
			player_->leftArmObject_.worldTransform_.translate_.y, 0.8f, animSpeed);
	}
}

void FloorGamePlayerAnimationManager::WalkAnimation() {
	float groundOffsetY = 0.1f;
	float walkSpeed = 20.0f;
	float animSpeed = 0.1f;

	float targetBodyY = 0.5f + (groundOffsetY * sinf(animationTimer_ * walkSpeed));
	player_->body_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(
		player_->body_.worldTransform_.translate_.y, targetBodyY, animSpeed);

	if (player_->isStriptting_ || player_->isReqestStript_) {
		player_->rightArmObject_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(
			player_->rightArmObject_.worldTransform_.translate_.y, 0.8f, animSpeed);
		player_->leftArmObject_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(
			player_->leftArmObject_.worldTransform_.translate_.y, 0.8f, animSpeed);
	} else {
		float targetBodyScaleY = 1.0f + (groundOffsetY * sinf(animationTimer_ * walkSpeed));
		float targetBodyScaleX = 1.0f + (0.1f * cosf(animationTimer_ * walkSpeed));
		player_->body_.worldTransform_.scale_.y = MY_Utility::SimpleEaseIn(
			player_->body_.worldTransform_.scale_.y, targetBodyScaleY, animSpeed);
		player_->body_.worldTransform_.scale_.x = MY_Utility::SimpleEaseIn(
			player_->body_.worldTransform_.scale_.x, targetBodyScaleX, animSpeed);

		player_->rightArmObject_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(
			player_->rightArmObject_.worldTransform_.translate_.y, 0.0f, animSpeed);
		player_->leftArmObject_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(
			player_->leftArmObject_.worldTransform_.translate_.y, 0.0f, animSpeed);

		float targetRightArmZ = 0.5f * sinf(animationTimer_ * walkSpeed + 3.14f);
		float targetLeftArmZ = 0.5f * sinf(animationTimer_ * walkSpeed);
		player_->rightArmObject_.worldTransform_.translate_.z = MY_Utility::SimpleEaseIn(
			player_->rightArmObject_.worldTransform_.translate_.z, targetRightArmZ, animSpeed);
		player_->leftArmObject_.worldTransform_.translate_.z = MY_Utility::SimpleEaseIn(
			player_->leftArmObject_.worldTransform_.translate_.z, targetLeftArmZ, animSpeed);
	}

	float targetRightLegZ = 0.25f * sinf(animationTimer_ * walkSpeed);
	float targetLeftLegZ = 0.25f * sinf(animationTimer_ * walkSpeed + 3.14f);
	player_->rightLegObject_.worldTransform_.translate_.z = MY_Utility::SimpleEaseIn(
		player_->rightLegObject_.worldTransform_.translate_.z, targetRightLegZ, animSpeed);
	player_->leftLegObject_.worldTransform_.translate_.z = MY_Utility::SimpleEaseIn(
		player_->leftLegObject_.worldTransform_.translate_.z, targetLeftLegZ, animSpeed);

	float targetRightLegRotX = 0.5f * sinf(animationTimer_ * walkSpeed);
	float targetLeftLegRotX = 0.5f * sinf(animationTimer_ * walkSpeed + 3.14f);
	player_->rightLegObject_.worldTransform_.rotate_.x = MY_Utility::SimpleEaseIn(
		player_->rightLegObject_.worldTransform_.rotate_.x, targetRightLegRotX, animSpeed);
	player_->leftLegObject_.worldTransform_.rotate_.x = MY_Utility::SimpleEaseIn(
		player_->leftLegObject_.worldTransform_.rotate_.x, targetLeftLegRotX, animSpeed);
}

void FloorGamePlayerAnimationManager::StriptAnimation() {
	
	if (player_->body_.worldTransform_.scale_.y >= 1.0f) {
		player_->animationState_ = PlayerAnimationState::Idle;
	}
}

void FloorGamePlayerAnimationManager::ShotAnimation() {
	

	if (player_->body_.worldTransform_.scale_.y >= 1.0f) {
		player_->animationState_ = PlayerAnimationState::Idle;
	}
}
