#pragma once

#include "Medjed/Enemy.h"
#include "BeamManager.h"

#include"Player/Player.h"
#include<memory>

#include "ParticleEmitter.h"
class RaySprite;

class ShotBeamManager
{
public:
	ShotBeamManager(Enemy* enemy, Player* player,BeamManager* beamManager);
	void Initialize();
	void Update();
	void RayCastHit(RaySprite& raySprite);
private:
	void CreateParticleEmitter();
private:

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	BeamManager* beamManager_ = nullptr;
	std::array<std::unique_ptr < ParticleEmitter>,2> beamParticleEmitters_;
	std::unique_ptr<ParticleEmitter> shockEmitter_ = nullptr;
	float currentTime_ = 0.0f;
	float tMin_ = 0.0f;
	float tMax_ = 1.0f;

	bool IntersectsAABB(const Ray& ray, const AABB& aabb, const Vector3& pos, const float kMaxDistance);
};

