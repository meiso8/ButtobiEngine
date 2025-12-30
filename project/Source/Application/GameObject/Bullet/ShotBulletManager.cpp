#include "ShotBulletManager.h"

#include "Medjed/Enemy.h"
#include "BulletManager.h"

#include"Sound.h"
#include"CollisionManager.h"
#include"Input.h"


ShotBulletManager::ShotBulletManager(Enemy* enemy, BulletManager* bulletManager, RhythmManager* rhythmManager)
    :enemy_(enemy), bulletManager_(bulletManager), rhythmManager_(rhythmManager)
{
}

void ShotBulletManager::Initialize() {
    currentTime_ = 0.0f;
    isReflect_ = false;
}

// Update内で時間を進める
void ShotBulletManager::Update() {
    isReflect_ = false;
    currentTime_ += InverseFPS;

    rhythmManager_->Update();

    if (enemy_->isShotStart_) {
        if (rhythmManager_->IsOnBeat(currentTime_)) {
            Sound::PlaySE(Sound::FIRE_BALL);
            Vector3 shotDirection = enemy_->GetToTarget();
            Vector3 shotPosition = enemy_->GetWorldPosition() + shotDirection * 0.5f;
            bulletManager_->ShotBullet(shotPosition, shotDirection, shotSpeed_, shotSize_, Bullet::kEnemy);
        }
    }
}

void ShotBulletManager::CheckRayHit(RaySprite& raySprite)
{

    for (auto& bullet : bulletManager_->GetBullets()) {
        if (!bullet->isActive_ && bullet->type_ != Bullet::kPlayer) { continue; }

        AABB aabb = GetAABBWorldPos(bullet.get());

        if (raySprite.IntersectsAABB(aabb, bullet->GetWorldPosition())) {

            if (Input::IsTriggerMouse(0)) {
                if (bullet->type_ != Bullet::kPlayer) {
                    Sound::PlaySE(Sound::FIRE_BALL);
                    Vector3 shotDirection = raySprite.ray_.diff;
                    Vector3 shotPosition = raySprite.ray_.origin + shotDirection * 1.0f;
                    bullet->Shot(shotPosition, shotDirection, shotSpeed_, shotSize_, Bullet::kPlayer);
                }
            }
        }
    }
}
