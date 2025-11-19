#include "FloorPlayerShotBulletManager.h"
#include "FloorGamePlayer.h"
#include "FloorBulletManager.h"
#include"Sound.h"
FloorPlayerShotBulletManager::FloorPlayerShotBulletManager(FloorGamePlayer* player, FloorBulletManager* bulletManager) :
	player_(player), bulletManager_(bulletManager) {
}

void FloorPlayerShotBulletManager::Initialize() {

}

void FloorPlayerShotBulletManager::Update() {
	if (player_ == nullptr || bulletManager_ == nullptr) {
		return;
	}

	// 床投げ
	if (player_->isReqestShot_) {
		Sound::PlaySE(Sound::THROW_FLOOR);
		player_->isReqestShot_ = false;
		// プレイヤーの向いている方向に床を投げる
		Vector3 shotDirection = player_->GetLookDir();
		Vector3 shotPosition = player_->body_.worldTransform_.translate_ + shotDirection * 0.5f;
		bulletManager_->ShotBullet(shotPosition, shotDirection, shotSpeed_, shotSize_);

		player_->body_.worldTransform_.scale_ = { 1.5f,0.1f,1.5f };
		player_->animationState_ = PlayerAnimationState::Shot;
	}
}
