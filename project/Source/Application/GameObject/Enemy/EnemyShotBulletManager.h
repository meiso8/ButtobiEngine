
#pragma once
class Enemy;
class EnemyBulletManager;

class EnemyShotBulletManager
{
public:
	EnemyShotBulletManager() = delete;
	EnemyShotBulletManager(Enemy* enemy, EnemyBulletManager* bulletManager);
	void Initialize();
	void Update();
private:
	Enemy* enemy_ = nullptr;
	EnemyBulletManager* bulletManager_ = nullptr;
	float shotSpeed_ = 0.3f;
	float shotSize_ = 0.5f;
};