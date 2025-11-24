#pragma once
#include"HPGage.h"
#include"HPIcon.h"
#include<unordered_map>
#include"PauseScreen.h"

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
    bool isDrawPlayerGage_ = false;
private:
    std::unordered_map<GageType, std::unique_ptr<HPGage>>hpGages_;
    std::unique_ptr<HPIcon>playerHpIcon_= nullptr;
    std::unique_ptr<PauseScreen>pauseScreen_ = nullptr;
};

