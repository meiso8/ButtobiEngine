#pragma once
class FloorBulletManager;
class FloorGamePlayer;

class FloorPlayerShotBulletManager
{
public:
	FloorPlayerShotBulletManager() = delete;
	FloorPlayerShotBulletManager(FloorGamePlayer* player, FloorBulletManager* bulletManager);
	void Initialize();
	void Update();
private:
	FloorGamePlayer* player_ = nullptr;
	FloorBulletManager* bulletManager_ = nullptr;
	float shotSpeed_ = 0.3f;
	float shotSize_ = 0.5f;
};