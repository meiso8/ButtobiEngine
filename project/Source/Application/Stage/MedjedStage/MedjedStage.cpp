#include "MedjedStage.h"
#include"SoundManager/SoundManager.h"

MedjedStage::MedjedStage()
{
    backGround_ = std::make_unique<BackGround>();
    medjedManager_ = std::make_unique<MedjedManager>();
    rhythmBullet_ = std::make_unique<RhythmBullet>();
    rhythmBullet_->SetEnemy(medjedManager_->GetEnemy());
}

void MedjedStage::Initialize()
{

    SoundManager::InitMedjedScene();
    backGround_->Initialize();
    medjedManager_->Initialize();
    rhythmBullet_->Initialize();
    medjedManager_->SetTarget(player_->GetBodyPos());
    player_->Init();
    player_->SetBodyPos({ 0.0f, 0.0f, -15.0f });


    itemManager_->GenerateItems({ "SunRod" });

}

void MedjedStage::Update()
{

    medjedManager_->Update();
    backGround_->Update();


    if (FindMedjed()) {
        backGround_->UpdateApperMedjed();
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
    backGround_->Draw(camera);

    if (FindMedjed()) {
        rhythmBullet_->Draw(camera);
    }

    medjedManager_->Draw(camera);

}

void MedjedStage::CheckCollision(CollisionManager& collisionManager)
{
    //メジェドたちがヒットしているかどうか
    medjedManager_->RayCastHit(*player_->raySprite_);
    //弾がヒットしているかどうか
    rhythmBullet_->GetShotBulletManager()->RayCastHit(*player_->raySprite_);


    //メジェド探したかどうか
    if (FindMedjed()) {

        collisionManager.AddCollider(medjedManager_->GetMedjed());

        //巨大メジェド出現し、弾を打ってくる
        for (auto& bullet : rhythmBullet_->GetBulletManager()->GetBullets()) {
            if (bullet->isActive_) {
                collisionManager.AddCollider(bullet.get());
            }
        }
        collisionManager.AddCollider(medjedManager_->GetEnemy());
    } else {
        for (auto& locker : medjedManager_->GetAllMedjeds()) {
            collisionManager.AddCollider(locker.get());
        }

    }

    // 壁との当たり判定
    for (auto& [type, object] : backGround_->GetBuilding()->GetFieldPoses()) {
        collisionManager.AddCollider(object.get());
    }
}
