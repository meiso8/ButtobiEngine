#pragma once
#include"../Stage/Stage.h"
#include"Medjed/MedjedManager.h"
#include"Bullet/RhythmBullet.h"
#include"Player/Player.h"
#include"CollisionManager.h"

#include<memory>
class MedjedStage :public Stage
{
private:
    std::unique_ptr<MedjedManager>medjedManager_ = nullptr;
    std::unique_ptr<RhythmBullet>rhythmBullet_ = nullptr;

public:

    MedjedStage();
    void Initialize()override;
    void Update()override;
    void Draw(Camera& camera)override;
    void CheckCollision(CollisionManager& collisionManager)override;
    const bool& FindMedjed() { return medjedManager_->GetIsFindMedjed(); }
    const bool& MedjedDead() { return medjedManager_->GetIsEnemyDead(); }
    Medjed* GetMedjed() { return medjedManager_->GetMedjed(); };
    Enemy* GetEnemy() { return medjedManager_->GetEnemy(); };
};

