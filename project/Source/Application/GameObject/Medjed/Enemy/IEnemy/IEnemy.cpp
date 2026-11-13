#include "IEnemy.h"

IEnemy::STATE IEnemy::state_ = STATE::APPEAR;

IEnemy::~IEnemy()
{
}

void IEnemy::ChangeState(const STATE state)
{
    state_ = state;
}

IEnemy::STATE IEnemy::GetState()
{
    return state_;
}
