#pragma once

#include "Medjed/Enemy.h"
#include"Player/Player.h"
#include<memory>
#include "ParticleEmitter.h"

class RaySprite;
class BeamManager;

class ShotBeamManager
{
public:
	ShotBeamManager(const Enemy* enemy, Player* player);
	void Initialize();
	void Update();
	void RayCastHit(RaySprite& raySprite);
	void Draw();
private:
	void CreateParticleEmitter();
private:
	//プレイヤーは書換を行うため保留とする
	 Player* player_ = nullptr;
	const Enemy* enemy_ = nullptr;

	std::unique_ptr<BeamManager>beamManager_ = nullptr;

	std::array<std::unique_ptr < ParticleEmitter>,2> beamParticleEmitters_;
	std::unique_ptr<ParticleEmitter> shockEmitter_ = nullptr;
	float currentTime_ = 0.0f;
	float tMin_ = 0.0f;
	float tMax_ = 1.0f;

	bool IntersectsAABB(const Ray& ray, const AABB& aabb, const Vector3& pos, const float kMaxDistance);
};

