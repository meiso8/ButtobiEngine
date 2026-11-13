#pragma once

#include"../IEnemy/IEnemy.h"
class IdleEnemy :public IEnemy {
public:
    void Update(Enemy* enemy)override;
    void Enter(Enemy* enemy)override;
    void Exit(Enemy* enemy)override;
};