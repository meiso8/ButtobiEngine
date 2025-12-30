#include "EnemyShotBulletManager.h"

#include "Enemy.h"
#include "EnemyBulletManager.h"

#include"Sound.h"
#include"CollisionManager.h"
#include"Input.h"

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
        Sound::PlaySE(Sound::FIRE_BALL);
        enemy_->isShot_ = false;
        // playerに向かって投げる
        Vector3 shotDirection = enemy_->GetToTarget();
        Vector3 shotPosition = enemy_->GetWorldPosition() + shotDirection * 0.5f;
        bulletManager_->ShotBullet(shotPosition, shotDirection, shotSpeed_, shotSize_,EnemyBullet::kEnemy);
    }
}

void EnemyShotBulletManager::CheckRayHit(RaySprite& raySprite)
{
    for (auto& bullet : bulletManager_->GetBullets()) {

        AABB aabb = GetAABBWorldPos(bullet.get());

        if (raySprite.IntersectsAABB(aabb, bullet->GetWorldPosition())) {
            //Mouseをクリックしたら
            if (Input::IsTriggerMouse(0)) {
                Sound::PlaySE(Sound::FIRE_BALL);
               
                Vector3 shotDirection = raySprite.ray_.diff;
                Vector3 shotPosition = raySprite.ray_.origin + shotDirection * 1.0f;
                bullet->isActive_ = false;
                bulletManager_->ShotBullet(shotPosition, shotDirection, shotSpeed_, shotSize_, EnemyBullet::kPlayer);

            }
        }
    }
}
