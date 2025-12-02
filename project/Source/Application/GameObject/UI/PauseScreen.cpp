#include "PauseScreen.h"
#include"Input.h"
#include"Easing.h"
#include"Window.h"
#include<algorithm>
#include"DebugUI.h"
#include "MatsumotoObj/KeyBindConfig.h"
#include"Sound.h"
bool PauseScreen::isActive_ = false;
bool PauseScreen::isPause_ = false;
bool PauseScreen::isRetry = false;
bool PauseScreen::isBackToTitle = false;

PauseScreen::PauseScreen()
{
    for (int i = 0; i < kMaxLayer; ++i) {
        sprites_[i] = std::make_unique<Sprite>();
    }

    sprites_[kBlackScreen]->Create(Texture::WHITE_1X1, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kBlackScreen]->SetSize({ static_cast<float>(Window::GetClientWidth()),static_cast<float>(Window::GetClientHeight()) });

    sprites_[kPauseBG]->Create(Texture::PAUSING_BG, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });

    sprites_[kPausing]->Create(Texture::PAUSING, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });

    sprites_[kBackToGame]->Create(Texture::BUTTON_BACK_TO_GAME, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
    sprites_[kReTry]->Create(Texture::BUTTON_RETRY, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
    sprites_[kBackToTitle]->Create(Texture::BUTTON_BACK_TO_TITL, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
    sprites_[kButton]->Create(Texture::BUTTON_A, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
    sprites_[kConfirm]->Create(Texture::CONFIRM_UI, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });


    float width = static_cast<float>(Window::GetClientWidth());
    float height = static_cast<float>(Window::GetClientHeight());

    startPos_[kPauseBG] = { width,0.0f };
    endPos_[kPauseBG] = { width - sprites_[kPauseBG]->GetSize().x,0.0f };

    startPos_[kPausing] = { width+128.0f,64.0f };
    endPos_[kPausing] = { width - sprites_[kPausing]->GetSize().x - 24.0f,startPos_[kPausing].y };

    startPos_[kBackToGame] = { width + 256.0f,height * 0.5f - 32.0f };
    endPos_[kBackToGame] = { startPos_[kBackToGame].x - 128.0f - sprites_[kBackToGame]->GetSize().x,  startPos_[kBackToGame].y };

    startPos_[kReTry] = { width + 256.0f,startPos_[kBackToGame].y + 96.0f };
    endPos_[kReTry] = { startPos_[kReTry].x - 128.0f - sprites_[kReTry]->GetSize().x,startPos_[kReTry].y };

    startPos_[kBackToTitle] = { width + 256.0f,startPos_[kReTry].y + 96.0f };
    endPos_[kBackToTitle] = { startPos_[kBackToTitle].x - 128.0f - sprites_[kBackToTitle]->GetSize().x,  startPos_[kBackToTitle].y };

    startPos_[kButton] = { width + 128.0f,height - sprites_[kButton]->GetSize().y - 40.0f };
    endPos_[kButton] = { width * 0.5f + sprites_[kButton]->GetSize().x + 256.0f, height - sprites_[kButton]->GetSize().y - 40.0f };

    startPos_[kConfirm] = { width + 128.0f,height - sprites_[kConfirm]->GetSize().y - 64.0f };
    endPos_[kConfirm] = { endPos_[kButton].x + 96.0f,  height - sprites_[kConfirm]->GetSize().y - 64.0f };

    for (int i = kButton; i < sprites_.size(); ++i) {
        sprites_[i]->SetPosition(endPos_[i]);
    }

    sprites_[kBackToGame]->SetAnchorPoint({ 0.5f,0.5f });
    sprites_[kReTry]->SetAnchorPoint({ 0.5f,0.5f });
    sprites_[kBackToTitle]->SetAnchorPoint({ 0.5f,0.5f });



}
void PauseScreen::Initialize()
{
    isPause_ = false;
    isRetry = false;
    isBackToTitle = false;
    isActive_ = false;
    pauseTimer_ = 0.0f;
    scaleTheta_ = 0.0f;

    for (int i = kPauseBG; i < sprites_.size(); ++i) {
        sprites_[i]->SetPosition(startPos_[i]);
    }

    selectButtonNum_ = kBackToGameButton;

    for (int i = 0; i < kButtonMax; ++i) {
        sprites_[i + kBackToGame]->SetScale({ 1.0f,1.0f });
    }
}
void PauseScreen::Update()
{

    if (KeyBindConfig::Instance().IsTrigger("Menu")) {
        Sound::IsPlaying(Sound::kPauseButton);
        isPause_ = (isPause_) ? false : true;
        pauseTimer_ = 0.0f;
        isActive_ = true;
        selectButtonNum_ = kBackToGameButton;
    }

    for (int i = 0; i < sprites_.size(); ++i) {
        std::string world = std::to_string(i);
        DebugUI::CheckSprite(*sprites_[i], world.c_str());
    }

    if (!isActive_) { return; }

    TimerUpdate();

    if (isPause_) {

        SelectButton();

        if (pauseTimer_ == 1.0f) {
            ScalingButton();
        }

        for (int i = kPauseBG; i < sprites_.size(); ++i) {
            Vector2 pos = Easing::EaseInOut(sprites_[i]->GetPosition(), endPos_[i], 0.2f);
            sprites_[i]->SetPosition(pos);
        }

    } else {

        for (int i = 0; i < kButtonMax; ++i) {
            sprites_[i + kBackToGame]->SetScale({ 1.0f,1.0f });
        }
        for (int i = kPauseBG; i < sprites_.size(); ++i) {
            Vector2 pos = Easing::EaseInOut(sprites_[i]->GetPosition(), startPos_[i], 0.2f);
            sprites_[i]->SetPosition(pos);
        }

        if (pauseTimer_ == 1.0f) {
            isActive_ = false;
        }
    }

}

void PauseScreen::TimerUpdate()
{
    if (pauseTimer_ == 1.0f) {
        return;
    }

    pauseTimer_ += InverseFPS;
    pauseTimer_ = std::clamp(pauseTimer_, 0.0f, 1.0f);
}

void PauseScreen::SelectButton()
{
	KeyBindConfig* key = &KeyBindConfig::Instance();
    if (key->IsTrigger("MoveForward")) {
        Sound::IsPlaying(Sound::kMoveCursor);
        selectButtonNum_--;
        if (selectButtonNum_ < 0) {
            selectButtonNum_ = kButtonMax - 1;
        }
        scaleTheta_ = 0.0f;
    }

    if (key->IsTrigger("MoveBack")) {
        Sound::IsPlaying(Sound::kMoveCursor);
        selectButtonNum_++;
        selectButtonNum_ %= kButtonMax;
        scaleTheta_ = 0.0f;
    }


    if (key->IsTrigger("Shot")) {
        Sound::IsPlaying(Sound::kDecision);
        switch (selectButtonNum_)
        {
        case kBackToGameButton:
            isActive_ = true;
            isPause_ = false;
            pauseTimer_ = 0.0f;

            break;
        case kReTryButton:
            isRetry = true;
            isPause_ = false;
            break;
        case kBackToTitleButton:
            isBackToTitle = true;
            break;

        }
    }

}

void PauseScreen::ScalingButton()
{
    for (int i = 0; i < kButtonMax; ++i) {
        if (selectButtonNum_ == i) {
            scaleTheta_ += InverseFPS * std::numbers::pi_v<float>*2.0f;
            float scale = cosf(scaleTheta_) * 0.125f + 1.125f;
            sprites_[i + kBackToGame]->SetScale({ scale,scale });
        } else {
            sprites_[i + kBackToGame]->SetScale({ 1.0f,1.0f });
        }
    }


}

void PauseScreen::Draw()
{
    if (!isActive_) { return; }

    sprites_[kButton]->PreDraw();

    for (auto& sprite : sprites_) {
        sprite->Draw();
    }

}
