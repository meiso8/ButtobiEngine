#pragma once
#include"FloorBullet.h"
#include"Data/FloorData.h" 
#include <vector>
class Enemy;
class FloorBulletManager
{
public:
	FloorBulletManager() = delete;
	FloorBulletManager(Enemy* enemy);
	~FloorBulletManager();
	void Initialize();
	void Update();
	void Draw(class Camera& camera, const LightMode& lightType);
	void ShotBullet(const Vector3& position, const Vector3& direction, const float& speed, const float& size,FloorType type);
	const std::vector<std::unique_ptr<FloorBullet>>& GetBullets() const {
		return bullets_;
	};

private:
	Enemy* enemy_;
	std::vector<std::unique_ptr<FloorBullet>> bullets_;
};