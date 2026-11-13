#include "BeamEnemy.h"
#include"TimeManager.h"
#include"../Enemy.h"
#include"Easing.h"
namespace {
    constexpr float kBeamTime_ = 3.0f;
    constexpr float kBeamEndTime_ = 10.0f;
}


void BeamEnemy::Update(Enemy* enemy)
{
    
    timer_ += TimeManager::DeltaTime();

    //ずっとコチラを覗く
    enemy->Look();

    if (timer_ < kBeamTime_) {

        if (enemy->PoyoPoyoUpdateAndGetEnd(1.0f)) {

            //ぽよぽよ終わったら
            enemy->SetAnimation("Swing");
       
            enemy->SetIsShotStart(true);

        } else {
            //ぽよぽよしている時はコレ
            enemy->SetAnimation("Idle");
        }

    } else if (timer_ < kBeamEndTime_) {
        //うなずく
        //ショットを中止する
        enemy->SetIsShotStart(false);
        enemy->SetAnimation("Nod");
    } else {
        ChangeState(IEnemy::STATE::ROUND);
    }
}

void BeamEnemy::Enter(Enemy* enemy)
{
    enemy->InitPoyoTimer();
    timer_ = 0.0f;
}

void BeamEnemy::Exit(Enemy* enemy)
{
    enemy->SetIsShotStart(false);
    //一応最後にも入れておく
    enemy->InitPoyoTimer();
}
