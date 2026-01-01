#include "GoldHeart.h"
#include"Lerp.h"

void GoldHeart::Update()
{
    if (used_) {
        UpdateAniTimer(2.0f);
        float localTimer = aniTimer_ * 0.5f;
        object_->worldTransform_.translate_ = Lerp(startPos_, endPos_, localTimer);
    }

    object_->Update();
    object_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
    ColliderUpdate();
}

void GoldHeart::Use()
{
    aniTimer_ = 0.0f;
    used_ = true;
    //ミイラにはめ込む
}
