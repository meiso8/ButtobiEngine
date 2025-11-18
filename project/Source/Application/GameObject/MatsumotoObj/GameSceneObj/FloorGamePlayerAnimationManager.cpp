#include "FloorGamePlayerAnimationManager.h"
#include "FloorGamePlayer.h"
#include "FloorGameFloorManager.h"

#include "MatsumotoObj/MY_Utility.h"

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

	animationTimer_ += deltaTime_;
	animationStateAction_[player_->animationState_]();
}

void FloorGamePlayerAnimationManager::IdleAnimation() {
	float groundOffsetY = 0.25f;

	player_->body_.worldTransform_.translate_.y = 0.5f + (groundOffsetY * sinf(animationTimer_ * 6.0f));

	player_->rightLegObject_.worldTransform_.rotate_.x = 0.1f * sinf(animationTimer_ * 6.0f + 3.14f);
	player_->leftLegObject_.worldTransform_.rotate_.x = 0.1f * sinf(animationTimer_ * 6.0f + 3.14f);

	if (player_->isStriptting_ || player_->isReqestStript_) {
		player_->rightArmObject_.worldTransform_.translate_.y = 0.5f;
		player_->leftArmObject_.worldTransform_.translate_.y = 0.5f;
	} else {
		player_->rightArmObject_.worldTransform_.translate_.y = 0.0f;
		player_->leftArmObject_.worldTransform_.translate_.y = 0.0f;

		player_->body_.worldTransform_.scale_.y = 1.0f + (groundOffsetY * sinf(animationTimer_ * 6.0f));
		player_->body_.worldTransform_.scale_.x = 1.0f + (0.1f * sinf(animationTimer_ * 6.0f));
	}
}

void FloorGamePlayerAnimationManager::WalkAnimation() {
	float groundOffsetY = 0.1f;
	float walkSpeed = 20.0f;

	
	player_->body_.worldTransform_.translate_.y = 0.5f + (groundOffsetY * sinf(animationTimer_ * walkSpeed));

	if (player_->isStriptting_ || player_->isReqestStript_) {
		player_->rightArmObject_.worldTransform_.translate_.y = 0.5f;
		player_->leftArmObject_.worldTransform_.translate_.y = 0.5f;
	} else {
		player_->body_.worldTransform_.scale_.y = 1.0f + (groundOffsetY * sinf(animationTimer_ * walkSpeed));
		player_->body_.worldTransform_.scale_.x = 1.0f + (0.1f * cosf(animationTimer_ * walkSpeed));

		player_->rightArmObject_.worldTransform_.translate_.y = 0.0f;
		player_->leftArmObject_.worldTransform_.translate_.y = 0.0f;
		player_->rightArmObject_.worldTransform_.translate_.z = 0.5f * sinf(animationTimer_ * walkSpeed + 3.14f);
		player_->leftArmObject_.worldTransform_.translate_.z = 0.5f * sinf(animationTimer_ * walkSpeed);
	}
	

	player_->rightLegObject_.worldTransform_.translate_.z = 0.25f * sinf(animationTimer_ * walkSpeed);
	player_->leftLegObject_.worldTransform_.translate_.z = 0.25f * sinf(animationTimer_ * walkSpeed + 3.14f);
	player_->rightLegObject_.worldTransform_.rotate_.x = 0.5f * sinf(animationTimer_ * walkSpeed);
	player_->leftLegObject_.worldTransform_.rotate_.x = 0.5f * sinf(animationTimer_ * walkSpeed + (3.14f ));
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
