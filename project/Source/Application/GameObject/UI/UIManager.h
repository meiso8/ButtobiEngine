#pragma once
#include"HPGage.h"
#include"HPIcon.h"
#include<unordered_map>
#include"PauseScreen.h"
#include"RedPinchScreen.h"
class UIManager
{
    enum GageType {
        kEnemy,
        kPlayer,
    };
public:
    UIManager(HPs& enemyHp, HPs& enemyTotalHp, HPs& playerHp, bool& isPlayerHit, bool& isEnemyHit, bool& isEnemyKnockBack);
    void Initialize();
    void Update();
    void Draw();
    bool isDrawPlayerGage_ = false;
    bool* isGameOverPtr_ = nullptr;
    void DrawRedScreen();
    void SetGageColor(const Vector4& color);
private:
    std::unordered_map<GageType, std::unique_ptr<HPGage>>hpGages_;
    std::unique_ptr<HPIcon>playerHpIcon_= nullptr;
    std::unique_ptr<PauseScreen>pauseScreen_ = nullptr;
    std::unique_ptr<RedPinchScreen> redPinchScreen = nullptr;

};

