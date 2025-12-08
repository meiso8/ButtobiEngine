#include "UIManager.h"
#include"Window.h"
#include"PauseScreen.h"
UIManager::UIManager(HPs& enemyHp,HPs& enemyTotalHp, HPs& playerHp,bool& isPlayerHit,bool&isEnemyHit,bool& isEnemyKnockBack)
{

    hpGages_.emplace(GageType::kEnemy, std::make_unique<HPGage>());
    hpGages_[kEnemy]->SetHpPtr(&enemyHp);
    hpGages_[kEnemy]->Setting({ 512.0f,32.0f }, { static_cast<float>(Window::GetClientWidth())-64.0f,80.0f}, { 1.0f,1.0f });
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
    playerHpIcon_->Setting({ 64.0f,64.0f }, { 64.0f,static_cast<float>(Window::GetClientHeight() - 96) });

    enemyHpIcon_ = std::make_unique<HPIcon>();
    enemyHpIcon_->SetMaxIconNum(3);
    enemyHpIcon_->SetHpPtr(&enemyTotalHp);
    enemyHpIcon_->SetIsHitPtr(isEnemyHit);

    enemyHpIcon_->SetTextureHandle(Texture::ENEMY_ACTION_PARTICLE);
    enemyHpIcon_->Setting({ 64.0f,64.0f }, { static_cast<float>(Window::GetClientWidth()) - 512.0f-64.0f,8.0f });

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
    enemyHpIcon_->Initialize();
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
        enemyHpIcon_->Update();
        redPinchScreen->Update();
    }

    pauseScreen_->Update();
 
}

void UIManager::Draw()
{

    redPinchScreen->Draw();

    for (const auto& [type, gage] : hpGages_) {
        gage->Draw();
    }

    playerHpIcon_->Draw();
    enemyHpIcon_->Draw();


    pauseScreen_->Draw();
}
