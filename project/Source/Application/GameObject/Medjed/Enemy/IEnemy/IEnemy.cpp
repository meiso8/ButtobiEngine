#include "IEnemy.h"
#include"../Enemy.h"
IEnemy::STATE IEnemy::state_ = STATE::APPEAR;

IEnemy::~IEnemy()
{
}

IEnemy::STATE IEnemy::GetState()
{
    return state_;
}

void IEnemy::ChangeState(const STATE state, Enemy* enemy)
{
    state_ = state;
    enemy->ChangeState(state);
}
