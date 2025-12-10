#include "UIManager.h"
#include"Window.h"
#include"PauseScreen.h"

UIManager::UIManager(HPs& enemyHp,HPs& enemyTotalHp, HPs& playerHp,bool& isPlayerHit,bool&isEnemyHit,bool& isEnemyKnockBack)
{

    hpGages_.emplace(GageType::kEnemy, std::make_unique<HPGage>());
    hpGages_[kEnemy]->SetHpPtr(&enemyHp);
    hpGages_[kEnemy]->Setting({ 512.0f*1.25f,32.0f*1.25f }, { static_cast<float>(Window::GetClientWidth())-32.0f,80.0f}, { 1.0f,1.0f });
    hpGages_[kEnemy]->isHitPtr_ = &isEnemyHit;
    hpGages_[kEnemy]->isKnockBackPtr_ = &isEnemyKnockBack;
#ifdef _DEBUG
    isDrawPlayerGage_ = true;
#endif

    if (isDrawPlayerGage_) {
        hpGages_.emplace(GageType::kPlayer, std::make_unique<HPGage>());
        hpGages_[kPlayer]->SetHpPtr(&playerHp);
        hpGages_[kPlayer]->Setting({ 256.0f,32.0f }, { 64.0f, static_cast<float>(Window::GetClientHeight() - 48) }, { 0.0f,0.0f });
    }

    playerHpIcon_ = std::make_unique<HPIcon>();
    playerHpIcon_->SetHpPtr(&playerHp);
    playerHpIcon_->SetIsHitPtr(isPlayerHit);
    playerHpIcon_->SetTextureHandle(Texture::PLAYER_HP_ICON);
    playerHpIcon_->Setting({ 64.0f*1.2f,64.0f*1.2f }, { 32.0f,20.0f});


    pauseScreen_ = std::make_unique<PauseScreen>();

    redPinchScreen = std::make_unique<RedPinchScreen>();
    redPinchScreen->SetHitPtr(isPlayerHit);
    redPinchScreen->hpPtr_ = &playerHp;
}

void UIManager::Initialize()
{
    for (const auto& [type, gage] : hpGages_) {
        gage->Initialize();
    }

    playerHpIcon_->Initialize();

    pauseScreen_->Initialize();
    redPinchScreen->Init();
}

void UIManager::Update()
{

    if (!PauseScreen::isActive_) {
        
        for (const auto& [type, gage] : hpGages_) {
            gage->Update();
            gage->UpdateHitAction();
        }
 
        playerHpIcon_->Update();

        redPinchScreen->Update();
    }

    if (isGameOverPtr_ && !*isGameOverPtr_) {
        pauseScreen_->Update();
    }

 
}

void UIManager::Draw()
{

    for (const auto& [type, gage] : hpGages_) {
        gage->Draw();
    }

    playerHpIcon_->Draw();

    if (isGameOverPtr_ && !*isGameOverPtr_) {
        pauseScreen_->Draw();
    }
}

void UIManager::DrawRedScreen()
{
    redPinchScreen->Draw();
 
}

void UIManager::SetGageColor(const Vector4& color)
{
    hpGages_[kEnemy]->SetColor(color);
}
