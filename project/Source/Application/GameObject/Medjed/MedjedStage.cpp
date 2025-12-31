#include "MedjedStage.h"
#include"SoundManager/SoundManager.h"


MedjedStage::MedjedStage()
{
    medjedManager_ = std::make_unique<MedjedManager>();
    rhythmBullet_ = std::make_unique<RhythmBullet>();
    rhythmBullet_->SetEnemy(medjedManager_->GetEnemy());
}

void MedjedStage::Initialize()
{
    medjedManager_->Initialize();
    rhythmBullet_->Initialize();
    medjedManager_->SetTarget(player_->GetBodyPos());
}

void MedjedStage::Update()
{

    medjedManager_->Update();
    if (FindMedjed()) {

        if (medjedManager_->GetIsApperMedjed()) {
            //メジェド出現！
            SoundManager::ApperMedjedUpdate();
        }

        rhythmBullet_->Update();

    } else {
        SoundManager::NotFindMedjedUpdate();
    }

}

void MedjedStage::Draw(Camera& camera)
{
    medjedManager_->Draw(camera);
    rhythmBullet_->Draw(camera);
}

void MedjedStage::CheckCollision(CollisionManager* collisionManager)
{
    //メジェドたちがヒットしているかどうか
    medjedManager_->RayCastHit(*player_->raySprite_);
    //弾がヒットしているかどうか
    rhythmBullet_->GetShotBulletManager()->RayCastHit(*player_->raySprite_);

    // ========================//メジェド　見つかってないときとそうではないとき================================
    if (FindMedjed()) {

        collisionManager->AddCollider(medjedManager_->GetMedjed());

        //巨大メジェド出現し、弾を打ってくる
        for (auto& bullet : rhythmBullet_->GetBulletManager()->GetBullets()) {
            if (bullet->isActive_) {
                collisionManager->AddCollider(bullet.get());
            }
        }
        collisionManager->AddCollider(medjedManager_->GetEnemy());
    } else {
        for (auto& locker : medjedManager_->GetAllMedjeds()) {
            collisionManager->AddCollider(locker.get());
        }

    }
}
