#include "AlphaWalkEnemy.h"
#include"../Enemy.h"
#include"Easing.h"
#include"TimeManager.h"

namespace {
    constexpr float kAlphaWalkTime_ = 10.0f;
    constexpr float kAlphaWalkEndTime_ = kAlphaWalkTime_ + 1.0f;
    constexpr float kMoveSpeed_ = 2.0f;
}

void AlphaWalkEnemy::Update(Enemy* enemy)
{
    
    timer_ += TimeManager::DeltaTime();
    //常にプレイヤーを向く
    enemy->Look();

    if (timer_ < kAlphaWalkTime_) {

        if (timer_ <= 1.0f) {
            //マスク処理
            enemy->SetMaskVol(Easing::EaseInOutBack(0.0f, 1.0f, timer_));
        }

    } else if (timer_ < kAlphaWalkEndTime_) {

        float time =TimeManager::GetLocalTimer(timer_, kAlphaWalkTime_, kAlphaWalkEndTime_);
        time = std::clamp(time, 0.0f, 1.0f);
        enemy->SetMaskVol(Easing::EaseInOutBack(1.0f, 0.0f, time));
    } else {
        enemy->SetMaskVol(0.0f);
        ChangeState(IEnemy::STATE::BEAM);
    }

    if (enemy->GetTarget()) {

        Vector3 velocity = enemy->GetToTarget();
        const float deltaTime = TimeManager::DeltaTime();
        velocity *= deltaTime * kMoveSpeed_;
        //Y軸方向には移動しない
        auto translate = enemy->GetTranslate();
        translate.x += velocity.x;
        translate.z += velocity.z;
        enemy->SetTranslate(translate);
        enemy->SetTranslate(translate);
        //足音の更新処理をここで呼び出す
        enemy->SoundFootStep(SoundFactory::MEDJED_FOOT_STEP_SMALL);

    }

}

void AlphaWalkEnemy::Enter(Enemy* enemy)
{
    timer_ = 0.0f;
    //透明移動する
    enemy->SetAnimation("Walk");
}

void AlphaWalkEnemy::Exit(Enemy* enemy)
{
}
