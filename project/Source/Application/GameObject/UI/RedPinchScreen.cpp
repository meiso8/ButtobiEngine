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

    // 赤の強さとアルファをイージングで制御
    float intensity = Easing::EaseOutExpo(1.0f, 0.0f, timer_); // 1→0 に減衰
    sprite_->SetColor({ intensity, 0.0f, 0.0f, intensity}); // 半透明赤
    DebugUI::CheckSprite(*sprite_,"redSprite");

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

