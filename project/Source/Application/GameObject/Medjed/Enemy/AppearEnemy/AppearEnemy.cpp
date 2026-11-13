#include "AppearEnemy.h"
#include"../Enemy.h"
#include"Easing.h"
#include"TimeManager.h"

namespace {
    constexpr float kApperTime_ = 7.0f;
    constexpr float kApperEndTime_ = kApperTime_ + 2.0f;
}

void AppearEnemy::Update(Enemy* enemy)
{
    //時間を進める
    timer_+=TimeManager::DeltaTime();

    float time = timer_ / kApperTime_;
    time = std::clamp(time, 0.0f, 1.0f);

    enemy->Look();

    //0スケールからだんだん大きくなって現れる
    Vector3 scale = Easing::EaseInBounce(Math::ZERO, enemy->GetInitScale(), time);

    if (timer_ >= kApperEndTime_) {
        scale = enemy->GetInitScale();
        //仮に回転とする
        ChangeState(IEnemy::STATE::ROUND);
    }

    enemy->SetScale(scale);
}

void AppearEnemy::Enter(Enemy* enemy)
{
    //タイマーを初期化する
    timer_ = 0.0f;
    //アニメーション設定
    enemy->SetAnimation("Round");
}

void AppearEnemy::Exit(Enemy* enemy)
{
}
