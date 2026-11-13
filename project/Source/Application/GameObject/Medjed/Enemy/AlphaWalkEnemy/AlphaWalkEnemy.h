#pragma once
#include"../IEnemy/IEnemy.h"

class AlphaWalkEnemy:public IEnemy
{
public:
    void Update(Enemy* enemy)override;
    void Enter(Enemy* enemy)override;
    void Exit(Enemy* enemy)override;
private:
    float timer_ = 0.0f;
};

