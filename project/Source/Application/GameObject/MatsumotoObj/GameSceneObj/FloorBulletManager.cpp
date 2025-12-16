#include "FloorBulletManager.h"
#include "Enemy/Enemy.h"
namespace {
	const int kMaxBullets = 300;
}

FloorBulletManager::FloorBulletManager(Enemy* enemy) {
	for (int i = 0; i < kMaxBullets; i++) {
		bullets_.emplace_back(std::make_unique<FloorBullet>());
		bullets_[i]->Initialize();
	}
	enemy_ = enemy;
}

FloorBulletManager::~FloorBulletManager() {
}

void FloorBulletManager::Initialize() {
	for (auto& bullet : bullets_) {
		bullet->Initialize();
	}
}

void FloorBulletManager::Update() {
	if (FloorFlag::isHitMultipleFloor) {
		if (enemy_ == nullptr) {
			return;
		}

		float homingStayTime = 0.2f;
		float addTime = 0.1f;
		for (auto& bullet : bullets_) {
			if (bullet->isActive_) {
				bullet->SetHoming(&enemy_->bodyPos_, homingStayTime);
				homingStayTime += addTime;
				addTime *= 0.85f;
			}
		}
		FloorFlag::isHitMultipleFloor = false;
	}
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void FloorBulletManager::Draw(Camera& camera) {
	for (auto& bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void FloorBulletManager::ShotBullet(const Vector3& position, const Vector3& direction, const float& speed, const float& size, FloorType type) {
	for (auto& bullet : bullets_) {
		if (!bullet->isActive_) {
			bullet->Shot(position, direction, speed, size,type);
			break;
		}
	}
}

