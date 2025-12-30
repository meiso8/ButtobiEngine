#pragma once
#include"Enemy/Enemy.h"

#include "Enemy/EnemyBulletManager.h"
#include "Enemy/EnemyShotBulletManager.h"


class EnemyManager
{
private:
    std::unique_ptr<Enemy>enemy_ = nullptr;
    std::unique_ptr<EnemyBulletManager>enemyBulletManager_ = nullptr;
    std::unique_ptr<EnemyShotBulletManager>enemyShotBulletManager_ = nullptr;
public:
    EnemyManager();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void SetTarget(Vector3& target);
    EnemyBulletManager* GetBulletManager() { return enemyBulletManager_.get(); };
    Enemy* GetEnemy() { return enemy_.get(); }
    EnemyShotBulletManager* GetEnemyShotBulletManager() { return enemyShotBulletManager_.get();}
};

