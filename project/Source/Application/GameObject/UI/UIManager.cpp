#include "UIManager.h"
#include"Window.h"
#include"ItemManager/ItemManager.h"
#include"Input.h"
#include"EyeCatch/EyeCatch.h"

UIManager::UIManager()
{
    effectSprite_ = std::make_unique<Sprite>();
    effectSprite_->Create(TextureFactory::WHITE_1X1, { 0.0f,0.0f }, { 1.0f,0.75f,0.75f,1.0f });
    effectSprite_->SetSize({ 1280.0f,720.0f });

    buttonSprite_ = std::make_unique<ButtonSprite>();

    curPos_ = std::make_unique<CurPos>();
    curPos_->Initialize();

    pauseScreen_ = std::make_unique<PauseScreen>();
    pauseScreen_->SetCurPosPtr(curPos_->GetScreenPosPtr());

    eyeCatch_ = std::make_unique<EyeCatch>();

}

UIManager::~UIManager()
{
    hpGauges_.clear();

}

void UIManager::Initialize()
{
    for (const auto& [type, gauge] : hpGauges_) {
        gauge->Initialize();
    }

    buttonSprite_->Initialize();
    pauseScreen_->Initialize();
    curPos_->Initialize();
    eyeCatch_->Initialize();
}

void UIManager::UpdateGage()
{
    for (const auto& [type, gauge] : hpGauges_) {
        gauge->Update();
    }


}

void UIManager::UpdatePauseScreen()
{
    buttonSprite_->Update();

    if (pauseScreen_->GetIsActive()) {
        curPos_->Update();
    } else {
        curPos_->Initialize();
#ifdef _RELEASE
        if(!Input::IsPressKey(DIK_ESCAPE)) {
            Input::SetCursorPosition(*curPos_->GetScreenPosPtr());
        }
   
#endif // _RELEASE

    }

    pauseScreen_->Update();

    if (buttonSprite_->IsThermographyCommandFirst()) {
        //最初にサーモグラフィーが行われたら
        
        //アイキャッチアニメーションを再生する
        eyeCatch_->Update();
    }

}

void UIManager::DrawHPGauge()
{
    for (const auto& [type, gauge] : hpGauges_) {
        gauge->Draw();
    }

}

void UIManager::DrawPauseScreen()
{
    pauseScreen_->Draw();


}

void UIManager::DrawCurPos()
{
    Sprite::PreDraw();

    buttonSprite_->Draw();

    if (buttonSprite_->IsThermographyCommandFirst()) {
        //最初にサーモグラフィーが行われたらで一応囲うが後で修正するかもしれない。
        eyeCatch_->Draw();
    }

    if (pauseScreen_->GetIsActive()) {
        curPos_->Draw();
    }


}

void UIManager::DrawEffect()
{
    Sprite::PreDraw(kBlendModeMultiply);
    effectSprite_->Draw();
}

void UIManager::CreateHpGage(const HPs& enemyHp, const HPs& playerHp)
{
    hpGauges_.emplace(GageType::kEnemy, std::make_unique<HPGauge>());
    hpGauges_[kEnemy]->SetHpPtr(&enemyHp);
    hpGauges_[kEnemy]->Setting({ 640.0f,16.0f }, { 640.0f,32.0f }, { 0.5f,0.0f });

    hpGauges_.emplace(GageType::kPlayer, std::make_unique<HPGauge>());
    hpGauges_[kPlayer]->SetHpPtr(&playerHp);
    hpGauges_[kPlayer]->Setting({ 640.0f,16.0f }, { 640.0f,static_cast<float>(Window::GetClientHeight()) - 136.0f }, { 0.5f,0.0f });

}


