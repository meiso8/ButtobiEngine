#include "ShotEnemy.h"
#include"TimeManager.h"
#include"../Enemy.h"
#include"Easing.h"
namespace {
    constexpr float kShotTime_ = 10.0f;
}

void ShotEnemy::Update(Enemy* enemy)
{
    timer_ += TimeManager::DeltaTime();
    float theta = timer_ * Math::kPi*2.0f;
    enemy->SetMaskVol(cosf(theta) * 0.5f + 0.5f);

    if (timer_ <= 1.0f) {
        enemy->RotateY(startRotateY_, endRotateY_,timer_);
    } else {
        enemy->SetIsShotStart(true);
        enemy->Look();
    }

    if (enemy->GetCurrentAnimation() == "Jump" || enemy->GetCurrentAnimation() == "Step") {
        //足音の更新処理をここで呼び出す
        enemy->SoundFootStep(SoundFactory::MEDJED_JUMP);
    }

    if (timer_ >= kShotTime_) {
        //透明移動をする
        ChangeState(IEnemy::STATE::ALPHA_WALK);
    }
}

void ShotEnemy::Enter(Enemy* enemy)
{
    timer_ = 0.0f;

    auto& transform = enemy->GetWorldTransform().eTransform_;
    startRotateY_ = transform.rotate.y;
    //スタート位置から一回転した値を入れる
    endRotateY_ = startRotateY_ + Math::kPi * 2.0f;

    //ここを変更する
    int randNum = rand() % 4;
    if (randNum == 0) {
        enemy->SetAnimation("Swing");
    } else if (randNum == 1) {
        enemy->SetAnimation("Nod");
    } else if (randNum == 2) {
        enemy->SetAnimation("Step");
    } else if (randNum == 3) {
        enemy->SetAnimation("Jump");
    }

}

void ShotEnemy::Exit(Enemy* enemy)
{
    enemy->SetIsShotStart(false);
}
