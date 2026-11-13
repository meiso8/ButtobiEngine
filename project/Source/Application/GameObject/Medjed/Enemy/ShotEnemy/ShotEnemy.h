#pragma once

#include"../IEnemy/IEnemy.h"
class ShotEnemy :public IEnemy {
public:
    void Update(Enemy* enemy)override;
    void Enter(Enemy* enemy)override;
    void Exit(Enemy* enemy)override;
private:
    float timer_ = 0.0f;
    float startRotateY_ = 0.0f;
    float endRotateY_ = 0.0f;
};