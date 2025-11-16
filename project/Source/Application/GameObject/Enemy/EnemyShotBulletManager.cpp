#include "EnemyShotBulletManager.h"

#include "Enemy.h"
#include "EnemyBulletManager.h"

EnemyShotBulletManager::EnemyShotBulletManager(Enemy* enemy, EnemyBulletManager* bulletManager) :
	enemy_(enemy), bulletManager_(bulletManager) {
}

void EnemyShotBulletManager::Initialize() {

}

void EnemyShotBulletManager::Update() {
	if (enemy_ == nullptr || bulletManager_ == nullptr) {
		return;
	}

	// 弾投げ
	if (enemy_->isShot_) {
		enemy_->isShot_ = false;
		// playerに向かって投げる
		Vector3 shotDirection = enemy_->GetToTarget();
		Vector3 shotPosition = enemy_->bodyPos_.worldTransform_.translate_ + shotDirection * 0.5f;
		bulletManager_->ShotBullet(shotPosition, shotDirection, shotSpeed_, shotSize_);
	}
}
