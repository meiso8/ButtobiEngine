#include<algorithm>
#include "HPIcon.h"
#include"Easing.h"
#include"DebugUI.h"
#include"Input.h"

HPIcon::HPIcon()
{

    for (int i = 0; i < kMaxHPIcon_; ++i) {
        sprites_[SpriteTypes::layer1].emplace_back(std::make_unique<Sprite>());
        sprites_[SpriteTypes::MaxHp].emplace_back(std::make_unique<Sprite>());
    }

    for (auto& [type, spriteList] : sprites_) {
        for (auto& sprite : spriteList) {
            sprite->Create(Texture::HART, { 64.0f,128.0f }, { 0.0f,0.0f,0.0f,0.5f });
        }
    }

}

void HPIcon::Setting(const Vector2& size, const Vector2& pos)
{
    for (auto& [type, spriteList] : sprites_) {
        for (size_t i = 0; i < spriteList.size(); ++i) {

            const float spacing = 16.0f; // スプライト同士の間隔

            Vector2 offsetPos = {
                pos.x + i * (size.x + spacing),
                pos.y
            };
            spriteList[i]->SetSize(size);
            spriteList[i]->SetPosition(offsetPos);
        }
    }
}

void HPIcon::Initialize()
{
    timer_ = 0.0f;
    preHP = hps_->hp;

    for (auto& sprite : sprites_[layer1]) {
        sprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });
    }
    drawHpNum_ = kMaxHPIcon_;
}

void HPIcon::Update()
{
    if (hps_->hp > 0) {

        if (hps_->hp < preHP) {
            timer_ = maxTime_;
        }

        preHP = hps_->hp;

        if (timer_ > 0.0f) {
            timer_ -= InverseFPS;
            theta_ += std::numbers::phi_v<float>*InverseFPS * 12.0f;
            float alpha = cosf(theta_) * 0.5f + 1.0f;
            sprites_[layer1][(drawHpNum_ - 1) % kMaxHPIcon_]->SetColor({ 1.0f,1.0f,1.0f,alpha });
        } else {
            timer_ = 0.0f;
            theta_ = 0.0f;
            drawHpNum_ = static_cast<size_t>(hps_->hp / (hps_->maxHp / static_cast<float>(sprites_[layer1].size())) + 0.9f);
            drawHpNum_ = static_cast<size_t>(std::clamp(static_cast<float>(drawHpNum_), 0.0f, static_cast<float>(kMaxHPIcon_)));
        }

    } else {

        drawHpNum_ = 0;

    }


    int num = (int)drawHpNum_;
    DebugUI::CheckFloat(timer_, "ICONTimer");
    DebugUI::CheckInt(num, "num");


}

void HPIcon::Draw()
{

    sprites_[layer1][0]->PreDraw();


    for (auto& sprite : sprites_[MaxHp]) {
        sprite->Draw();
    }


    for (size_t i = 0; i < drawHpNum_; ++i) {
        sprites_[layer1][i]->Draw();
    }


}
