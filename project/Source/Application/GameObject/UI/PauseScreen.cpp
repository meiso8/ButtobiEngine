#include "PauseScreen.h"
#include"Input.h"
#include"Easing.h"
#include"Window.h"
PauseScreen::PauseScreen()
{
    enum Layer {
        kBlackScreen,
        kPauseBG,
        kPausing,
        kBackToGame,
        kReset,
        kBackToTitle,
        kConfirm,
        kButton,
        kMaxLayer,
    };


    for (int i = 0; i < kMaxLayer; ++i) {
        sprites_[i] = std::make_unique<Sprite>();
    }


    sprites_[kBlackScreen]->Create(Texture::WHITE_1X1, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kBlackScreen]->SetSize({static_cast<float>(Window::GetClientWidth()),static_cast<float>(Window::GetClientHeight())});
    sprites_[kPauseBG]->Create(Texture::UV_CHECKER, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });


}
void PauseScreen::Update()
{

    if (Input::IsTriggerKey(DIK_E)) {
        isPause_ = true;
        pauseTimer_ = 0.0f;
    }

    if (isPause_) {

        pauseTimer_ += InverseFPS;
        Easing::EaseInOut(startPos_, endPos_, pauseTimer_);
    }

}

void PauseScreen::Draw()
{
}
