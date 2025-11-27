#pragma once
#include "HealItem.h"

class CollisionManager;

class HealItemSpawner {
public:
	HealItemSpawner();
	~HealItemSpawner();
	void Initialize();
	void Update();
	void Draw(class Camera& camera, const LightMode& lightType);
	void SpawnHealItem(const Vector3& position);

	void AddCollider(CollisionManager* collisionManager);
private:
	std::vector<std::unique_ptr<HealItem>> healItems_;
	const int kMaxHealItemCount_ = 10;
};