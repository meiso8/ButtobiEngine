#include "RoundEnemy.h"
#include"TimeManager.h"
#include"../Enemy.h"
#include"Easing.h"

namespace {
    constexpr float kRoundTime = 5.0f;

}
void RoundEnemy::Update(Enemy* enemy)
{
    timer_+= TimeManager::DeltaTime();
    enemy->SetMaskVol(Easing::EaseInBounce(1.0f,0.0f,fmod(timer_,1.0f)));

    Vector3 translate = enemy->GetTranslate();
    translate = Lerp(translate, { 0.0f,0.0f,0.0f }, 0.5f);
    enemy->SetTranslate(translate);

    enemy->Look();
    if (timer_ >= kRoundTime) {
        ChangeState(IEnemy::STATE::SHOT);
    }
}

void RoundEnemy::Enter(Enemy* enemy)
{
    timer_ = 0.0f;
    //アニメーションの設定
    enemy->SetAnimation("Round");
}

void RoundEnemy::Exit(Enemy* enemy)
{
}
