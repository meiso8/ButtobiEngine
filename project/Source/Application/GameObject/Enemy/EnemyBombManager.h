#pragma once
#include"EnemyBomb.h"
#include <vector>
class EnemyBombManager
{
public:
	EnemyBombManager();
	~EnemyBombManager();
	void Initialize();
	void Update();
	void Draw(class Camera& camera, const LightMode& lightType);
	void ShotBomb(const Vector3& startPpos,const Vector3& endPos, const float& size);
	const std::vector<std::unique_ptr<EnemyBomb>>& GetBombs() const {
		return bombs_;
	};
	bool isBombReset_ = false;
private:
	std::vector<std::unique_ptr<EnemyBomb>> bombs_;
};