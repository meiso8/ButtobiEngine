#include "RedPinchScreen.h"
#include"Easing.h"
#include"Input.h"
#include"Window.h"
#include"DebugUI.h"
RedPinchScreen::RedPinchScreen()
{
    sprite_ = std::make_unique<Sprite>();
    sprite_->Create(Texture::BLOOM, { 0.0f,0.0f }, { 1.0f,0.0f,0.0f,1.0f });
    sprite_->SetSize({ static_cast<float>(Window::GetClientWidth()),
        static_cast<float>(Window::GetClientHeight()) });
}

void RedPinchScreen::Init()
{
    sprite_->SetColor({ 1.0f,0.0f,0.0f,0.0f });
    timer_ = 0.0f;
}

void RedPinchScreen::Update()
{
    if (*isHitPtr_) {
        timer_ = 0.0f;
    }

    TimerUpdate();

    float intensity = Easing::EaseOutExpo(1.0f, 0.0f, timer_); // 1→0 に減衰

    if (hpPtr_ && hpPtr_->hp <= 20.0f) {
        sprite_->SetColor({ 1.0f - intensity, 0.0f, 0.0f, timer_ * 0.4f }); // 半透明赤
    } else {
        // 赤の強さとアルファをイージングで制御
        sprite_->SetColor({ intensity, 0.0f, 0.0f, intensity * 0.5f }); // 半透明赤
    }

    DebugUI::CheckSprite(*sprite_, "redSprite");

}

void RedPinchScreen::TimerUpdate()
{
    if (timer_ == 1.0f) {
        return;
    }

    timer_ += InverseFPS;

    if (timer_ > 1.0f) {
        timer_ = 1.0f;
    }
}

void RedPinchScreen::Draw()
{
    Sprite::PreDraw();
    sprite_->Draw();
}

