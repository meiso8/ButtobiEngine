#include "RhythmBullet.h"

RhythmBullet::RhythmBullet()
{
    rhythmManager_ = std::make_unique<RhythmManager>();
    bulletManager_ = std::make_unique<BulletManager>();
}

void RhythmBullet::Initialize()
{
    rhythmManager_->Initialize();

    bulletManager_->Initialize();
    shotBulletManager_->Initialize();
}

void RhythmBullet::Update()
{
    rhythmManager_->Update();
    bulletManager_->Update();
    shotBulletManager_->Update();
}

void RhythmBullet::Draw(Camera& camera)
{
    bulletManager_->Draw(camera);
}

void RhythmBullet::SetEnemy(Enemy* enemy)
{
    shotBulletManager_ = std::make_unique<ShotBulletManager>(enemy, bulletManager_.get(), rhythmManager_.get());
}

