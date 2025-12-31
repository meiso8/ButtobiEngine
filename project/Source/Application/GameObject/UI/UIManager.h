#pragma once
#include"HPGage.h"
#include"HPIcon.h"
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
    void Update();
    void DrawHPGage();
    void DrawEffect();
    void CreateHpGage(HPs& enemyHp, HPs& playerHp);
private:
    std::unordered_map<GageType, std::unique_ptr<HPGage>>hpGages_;
    std::unique_ptr<Sprite> effectSprite_;
};

