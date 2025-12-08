#pragma once
#include "Sprite.h"
#include<memory>
#include"CharacterState.h"
class RedPinchScreen
{
public:
    RedPinchScreen();
    void Init();
    void Update();
    void TimerUpdate();
    void Draw();
    void SetHitPtr(bool& isHitPtr) {
        isHitPtr_ = &isHitPtr;
    };
    HPs* hpPtr_ = nullptr;
private:
   
    bool* isHitPtr_ = nullptr;
    std::unique_ptr<Sprite> sprite_;
    float timer_ = 0.0f;
};

