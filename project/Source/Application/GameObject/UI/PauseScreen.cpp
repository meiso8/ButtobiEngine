#include "PauseScreen.h"
#include"Input.h"
#include"Easing.h"
#include"Window.h"

bool PauseScreen::isPause_ = false;

PauseScreen::PauseScreen()
{
    for (int i = 0; i < kMaxLayer; ++i) {
        sprites_[i] = std::make_unique<Sprite>();
    }

    sprites_[kBlackScreen]->Create(Texture::WHITE_1X1, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kBlackScreen]->SetSize({ static_cast<float>(Window::GetClientWidth()),static_cast<float>(Window::GetClientHeight()) });
    sprites_[kPauseBG]->Create(Texture::UV_CHECKER, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kPausing]->Create(Texture::UV_CHECKER, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kBackToGame]->Create(Texture::UV_CHECKER, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kReset]->Create(Texture::UV_CHECKER, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kBackToTitle]->Create(Texture::UV_CHECKER, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kConfirm]->Create(Texture::UV_CHECKER, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kButton]->Create(Texture::UV_CHECKER, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });

}
void PauseScreen::Update()
{

    if (Input::IsTriggerKey(DIK_E)) {
        isPause_ = (isPause_) ? false : true;
        pauseTimer_ = 0.0f;
    }

    if (isPause_) {
        pauseTimer_ += InverseFPS;
        Easing::EaseInOut(startPos_, endPos_, pauseTimer_);
    }

}

void PauseScreen::Draw()
{
    if (isPause_) {
        sprites_[kButton]->PreDraw();
        for (auto& sprite : sprites_) {
            sprite->Draw();
        }
    }
}
