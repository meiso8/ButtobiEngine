#pragma once
#include"FloorBullet.h"
#include <vector>
class FloorBulletManager
{
public:
	FloorBulletManager();
	~FloorBulletManager();
	void Initialize();
	void Update();
	void Draw(class Camera& camera, const LightMode& lightType);
	void ShotBullet(const Vector3& position, const Vector3& direction, const float& speed, const float& size);
	const std::vector<std::unique_ptr<FloorBullet>>& GetBullets() const {
		return bullets_;
	};

private:
	std::vector<std::unique_ptr<FloorBullet>> bullets_;
};