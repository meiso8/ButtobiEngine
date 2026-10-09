#pragma once
#include"Medjed/Enemy.h"
#include "BulletManager.h"
#include "ShotBulletManager.h"
#include"RhythmManager.h"
#include<memory>
#include"../GameObject/Beam/BeamManager.h"
#include"../GameObject/Beam/ShotBeamManager.h"
#include"SoundFactory.h"

class RaySprite;
class Player;

class RhythmBullet
{
private:
    std::unique_ptr<ShotBulletManager>shotBulletManager_ = nullptr;
    std::unique_ptr<ShotBeamManager>shotBeamManager_ = nullptr;
public:
    RhythmBullet(const Enemy* enemy, Player* player);
    void SetSound(const SoundFactory::TAG tag);
    void Initialize();
    void Update();
    void Draw();

    const std::vector<std::unique_ptr<Bullet>>& GetBullet() { return  shotBulletManager_->GetBulletManager()->GetBullets(); }
    //BulletManager* GetBulletManager() { return bulletManager_.get(); };
    ShotBulletManager* GetShotBulletManager() { return shotBulletManager_.get();}
    ShotBeamManager* GetShotBeamManager() { return shotBeamManager_.get(); }


};

