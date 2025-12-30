#include"Player/RaySprite.h"
#include"RhythmManager.h"
#pragma once
class Enemy;
class BulletManager;

class ShotBulletManager
{
public:
	ShotBulletManager(Enemy* enemy, BulletManager* bulletManager, RhythmManager* rhythmManager);
	void Initialize();
	void Update();
	void CheckRayHit(RaySprite& raySprite);
private:
	Enemy* enemy_ = nullptr;
	BulletManager* bulletManager_ = nullptr;
	RhythmManager* rhythmManager_ = nullptr;

	float shotSpeed_ = 0.3f;
	float shotSize_ = 3.0f;
	float currentTime_ = 0.0f;

	bool isReflect_ = false;
};
