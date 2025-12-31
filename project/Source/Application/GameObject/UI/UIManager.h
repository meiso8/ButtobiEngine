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
    UIManager(HPs& enemyHp, HPs& playerHp);
    void Initialize();
    void Update();
    void Draw();

private:
    std::unordered_map<GageType, std::unique_ptr<HPGage>>hpGages_;
};

