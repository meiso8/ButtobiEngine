#pragma once
#include"HPGage.h"
#include"HPIcon.h"
#include"PauseScreen.h"

#include<unordered_map>

class UIManager
{
    enum GageType {
        kEnemy,
        kPlayer,
    };
public:
    UIManager();
    void Initialize();
    void UpdateGage();
    void UpdatePauseScreen();
    void DrawHPGage();
    void DrawPauseScreen();
    void DrawEffect();
    void CreateHpGage(HPs& enemyHp, HPs& playerHp);
private:
    std::unique_ptr<PauseScreen> pauseScreen_;
    std::unordered_map<GageType, std::unique_ptr<HPGage>>hpGages_;
    std::unique_ptr<Sprite> effectSprite_;
};

