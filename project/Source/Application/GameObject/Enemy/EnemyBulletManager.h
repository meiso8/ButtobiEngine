#pragma once
#include"EnemyBullet.h"
#include <vector>
class EnemyBulletManager
{
public:
	EnemyBulletManager();
	~EnemyBulletManager();
	void Initialize();
	void Update();
	void Draw(class Camera& camera, const LightMode& lightType);
	void ShotBullet(const Vector3& position, const Vector3& direction, const float& speed, const float& size, const EnemyBullet::BulletType& type);
	const std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() const {
		return bullets_;
	};

private:
	std::vector<std::unique_ptr<EnemyBullet>> bullets_;
};