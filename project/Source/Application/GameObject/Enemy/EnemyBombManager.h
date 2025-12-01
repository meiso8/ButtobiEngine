#pragma once
#include"EnemyBomb.h"
#include <vector>

class EnemyBombManager
{
public:
	static const int kMaxBombs = 20;
	EnemyBombManager();
	~EnemyBombManager();
	void Initialize();
	void Update();
	void Draw(class Camera& camera, const LightMode& lightType);
	void ShotBomb(const Vector3& startPpos,const Vector3& endPos, const float& size);
	const std::vector<std::unique_ptr<EnemyBomb>>& GetBombs() const {
		return bombs_;
	};
private:
	std::vector<std::unique_ptr<EnemyBomb>> bombs_;
};