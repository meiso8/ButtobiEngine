#include "UIManager.h"
#include"Window.h"
UIManager::UIManager(HPs& enemyHp, HPs& playerHp)
{
    hpGages_.emplace(GageType::kEnemy, std::make_unique<HPGage>());
    hpGages_[kEnemy]->SetHpPtr(&enemyHp);
    hpGages_[kEnemy]->Setting({ 640.0f,16.0f }, { 640.0f,32.0f }, { 0.5f,0.0f });

    hpGages_.emplace(GageType::kPlayer, std::make_unique<HPGage>());
    hpGages_[kPlayer]->SetHpPtr(&playerHp);
    hpGages_[kPlayer]->Setting({ 640.0f,16.0f }, { 640.0f,static_cast<float>(Window::GetClientHeight()) - 136.0f }, { 0.5f,0.0f });

}

void UIManager::Initialize()
{
    for (const auto& [type, gage] : hpGages_) {
        gage->Initialize();
    }

}

void UIManager::Update()
{

    for (const auto& [type, gage] : hpGages_) {
        gage->Update();
    }

}

void UIManager::Draw()
{
    for (const auto& [type, gage] : hpGages_) {
        gage->Draw();
    }

}
