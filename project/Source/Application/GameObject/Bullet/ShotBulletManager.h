#pragma once

#include"RaySprite/RaySprite.h"
#include"SoundFactory.h"
class Enemy;
class BulletManager;
class RhythmManager;

class ShotBulletManager
{
public:
	ShotBulletManager(const Enemy* enemy);
	void Initialize();
	void Update();
	void RayCastHit(RaySprite& raySprite);
	void Draw();
	void SetSound(const SoundFactory::TAG& tag);
	const BulletManager* GetBulletManager() { return bulletManager_.get(); }
private:
	const Enemy* enemy_ = nullptr;

	std::unique_ptr<RhythmManager>rhythmManager_ = nullptr;
	std::unique_ptr<BulletManager>bulletManager_ = nullptr;

	float shotSpeed_ = 0.3f;
	float shotSize_ = 1.5f;
	float currentTime_ = 0.0f;

};
