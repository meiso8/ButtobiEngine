#include "HPGage.h"
#include"Easing.h"
#include"DebugUI.h"
#include"Input.h"

HPGage::HPGage()
{
    sprites_.emplace(SpriteTypes::layer1, std::make_unique<Sprite>());
    sprites_.emplace(SpriteTypes::layer2, std::make_unique<Sprite>());
    sprites_.emplace(SpriteTypes::MaxHp, std::make_unique<Sprite>());


    sprites_[layer1]->Create(Texture::WHITE_1X1, { 512-32.0f,64.0f -32.0f}, { 1.0f,0.0f,0.0f,1.0f });
    sprites_[layer2]->Create(Texture::WHITE_1X1, { 512.0-32.0f,64.0f-32.0f }, { 1.0f,1.0f,1.0f,1.0f });
    sprites_[MaxHp]->Create(Texture::GAUGE01, { 512.0f,64.0f }, { 1.0f,1.0f,1.0f,1.0f });
    shake_ = std::make_unique<Shake>();
}

void HPGage::SetColor(const Vector4& color)
{
    sprites_[layer1]->SetColor(color);
}

void HPGage::Setting(const Vector2& size, const Vector2& pos, const Vector2& anchorPoint)
{
    for (const auto& [type, sprite] : sprites_) {
        if (type == MaxHp) {
            sprite->SetSize(size);
            sprite->SetPosition(pos);
        } else {
            float sizeOffset = size.y * 0.25f;
            float posOffset = sizeOffset * 0.5f;
            sprite->SetSize(size-Vector2{ sizeOffset,sizeOffset });

            if (anchorPoint.x == 0.0f&&anchorPoint.y == 0.0f) {
                sprite->SetPosition(pos + Vector2{ posOffset,posOffset });
            } else if(anchorPoint.x == 1.0f && anchorPoint.y == 1.0f){        
                sprite->SetPosition(pos - Vector2{ posOffset,posOffset });
            } else {
                sprite->SetPosition(pos);
            }
        }
     
        sprite->SetAnchorPoint(anchorPoint);
        sprite->Update();
        pos_[type] = sprite->GetPosition();
    }
}

void HPGage::Initialize()
{
    shake_->Reset();
    preHP_ = hps_->maxHp;
    timer_ = 0.0f;
    preScale_ = 0.0f;
}

void HPGage::Update()
{
    if (hps_->hp != preHP_) {
        preHP_ = hps_->hp;
        timer_ = 0.0f;
    }

    if (timer_ < 1.0f) {
        timer_ += InverseFPS * 2.0f;
    } else {
        timer_ = 1.0f;
    }

    float scale = Easing::EaseInCubic(preScale_, { static_cast<float>(hps_->hp) / static_cast<float>(hps_->maxHp) }, timer_);
    float preHPScale = Lerp(preScale_, { static_cast<float>(preHP_) / static_cast<float>(hps_->maxHp) }, 1.0f);
    sprites_[layer1]->SetScale({ preHPScale,1.0f });
    sprites_[layer2]->SetScale({ scale,1.0f });
    //sprites_[layer1]->SetColor({ 1.0f - (static_cast<float>(hps_->hp) / static_cast<float>(hps_->maxHp)) ,static_cast<float>(hps_->hp) / static_cast<float>(hps_->maxHp) ,0.0f,0.75f });

    preScale_ = scale;

    DebugUI::CheckSprite(*sprites_[layer1], "HPGage");
}

void HPGage::Draw()
{

    Sprite::PreDraw();

    for (const auto& [type, sprite] : sprites_) {
  
        sprite->Draw();
    }
  
}

void HPGage::UpdateHitAction()
{

    if (isKnockBackPtr_ && *isKnockBackPtr_) {
        shake_->Reset();
        shake_->Start(50.0f, 90);
    } else if (isHitPtr_ && *isHitPtr_) {
        shake_->Start(15.0f, 10);
    }

    shake_->Update();

    for (const auto& [type, sprite] : sprites_) {
        sprite->SetPosition(pos_[type] + shake_->GetOffset());
    }

}
