#include "RhythmBullet.h"
#include"RaySprite/RaySprite.h"

RhythmBullet::RhythmBullet(const Enemy* enemy,Player* player)
{
    shotBulletManager_ = std::make_unique<ShotBulletManager>(enemy);
    shotBeamManager_ = std::make_unique<ShotBeamManager>(enemy, player);
}

void RhythmBullet::SetSound(const SoundFactory::TAG tag)
{
    shotBulletManager_->SetSound(tag);
}

void RhythmBullet::Initialize()
{
    shotBulletManager_->Initialize();
    shotBeamManager_->Initialize();
}

void RhythmBullet::Update()
{
    shotBulletManager_->Update();
    //ビーム

    shotBeamManager_->Update();
}

void RhythmBullet::Draw()
{
    shotBulletManager_->Draw();
    shotBeamManager_->Draw();
}


