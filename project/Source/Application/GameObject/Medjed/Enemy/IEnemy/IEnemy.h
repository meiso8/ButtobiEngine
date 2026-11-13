#pragma once
class Enemy;


class IEnemy
{
public:
    enum class STATE {
        APPEAR,
        ROUND,
        SHOT,
        ALPHA_WALK,
        BEAM,
        IDLE,
        EXIT,
    };

private:
    static STATE state_;
public:

    virtual void Update(Enemy* enemy) = 0;
    virtual void Enter(Enemy* enemy) = 0;
    virtual void Exit(Enemy* enemy) = 0;

    virtual ~IEnemy();
    STATE GetState();
    void ChangeState(const STATE state);

};

