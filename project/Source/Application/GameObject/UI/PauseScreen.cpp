#include "PauseScreen.h"
#include"Input.h"
#include"Easing.h"
#include"Window.h"
#include<algorithm>
#include"DebugUI.h"
#include"InputBind.h"
#include"Collision.h"
#include"Puzzle/SlidePuzzleSystem.h"
#include"Sound.h"

#include"Memo/MemoManager.h"

bool PauseScreen::isActive_ = false;
bool PauseScreen::isPause_ = false;
bool PauseScreen::isBackToTitle = false;

PauseScreen::PauseScreen()
{
    for (int i = 0; i < kMaxLayer; ++i) {
        sprites_[i] = std::make_unique<Sprite>();
    }

    float width = static_cast<float>(Window::GetClientWidth());
    float height = static_cast<float>(Window::GetClientHeight());

    sprites_[kBlackScreen]->Create(Texture::WHITE_1X1, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,0.5f });
    sprites_[kBlackScreen]->SetSize({ width,height });

    sprites_[kBackToGame]->Create(Texture::BUTTON_BACK_TO_GAME, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
    sprites_[kBackToTitle]->Create(Texture::BUTTON_BACK_TO_TITL, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
    pos_[kBackToGame] = { width*0.5f,  528.0f + sprites_[kBackToGame]->GetSize().y * 0.5f };
    pos_[kBackToTitle] = { width - sprites_[kBackToTitle]->GetSize().x*0.5f-64.0f,  528.0f+sprites_[kBackToTitle]->GetSize().y * 0.5f };

    sprites_[kBackToGame]->SetAnchorPoint({ 0.5f,0.5f });
    sprites_[kBackToTitle]->SetAnchorPoint({ 0.5f,0.5f });

    for (int i = kBackToGame; i < sprites_.size(); ++i) {
        sprites_[i]->SetPosition(pos_[i]);
    }

}
void PauseScreen::Initialize()
{
    isPause_ = false;

    isBackToTitle = false;
    isActive_ = false;
    pauseTimer_ = 0.0f;
    scaleTheta_ = 0.0f;

    selectButtonNum_ = kBackToGame;

    for (int i = kBackToGame; i < kMaxLayer; ++i) {
        sprites_[i]->SetScale({ 1.0f,1.0f });
    }
}
void PauseScreen::Update()
{

    if (SlidePuzzleSystem::IsActive() || MemoManager::isLookItem_) {

        if (!isPause_) {
            isPause_ = true;
            pauseTimer_ = 0.0f;
            isActive_ = true;
            selectButtonNum_ = kBackToGame;
        }

    } else {
        isPause_ = false;
        isActive_ = false;
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

    } else {

        for (int i = kBackToGame; i < kMaxLayer; ++i) {
            sprites_[i]->SetScale({ 1.0f,1.0f });
        }

        isActive_ = false;

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
    Vector2 pos = Input::GetCursorPosition();

    for (int i = kBackToGame; i < kMaxLayer; ++i) {

        if (IsCollision(*sprites_[i], pos)) {
            sprites_[i]->SetColor({ 1.0f,0.0f,0.0f,1.0f });
            ScalingButton();
            selectButtonNum_ = i;

            if (InputBind::IsClick()) {

                switch (selectButtonNum_)
                {
                case kBackToGame:
                    isActive_ = true;
                    isPause_ = false;
                    pauseTimer_ = 0.0f;
                    SlidePuzzleSystem::SetActive(false);
                    MemoManager::isLookItem_ = false;
                    break;
                case kBackToTitle:
                    isBackToTitle = true;
                    break;
                }

            }

        } else {
            sprites_[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
        }
    }

}

void PauseScreen::ScalingButton()
{
    //for (int i = 0; i < kButtonMax; ++i) {
    //    if (selectButtonNum_ == i) {
    //        scaleTheta_ += InverseFPS * std::numbers::pi_v<float>*2.0f;
    //        float scale = cosf(scaleTheta_) * 0.125f + 1.125f;
    //        sprites_[i + kBackToGame]->SetScale({ scale,scale });
    //    } else {
    //        sprites_[i + kBackToGame]->SetScale({ 1.0f,1.0f });
    //    }
    //}
}

void PauseScreen::Draw()
{
    if (!isActive_) { return; }

    Sprite::PreDraw();
    for (auto& sprite : sprites_) {
        sprite->Draw();
    }

}
