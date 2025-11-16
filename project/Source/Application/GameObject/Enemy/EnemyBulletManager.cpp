#include "EnemyBulletManager.h"

namespace {
	const int kMaxBullets = 128;
}

EnemyBulletManager::EnemyBulletManager() {
	for (int i = 0; i < kMaxBullets; i++) {
		bullets_.emplace_back(std::make_unique<EnemyBullet>());
		bullets_[i]->Initialize();
	}
}

EnemyBulletManager::~EnemyBulletManager() {
}

void EnemyBulletManager::Initialize() {
	for (auto& bullet : bullets_) {
		bullet->Initialize();
	}
}

void EnemyBulletManager::Update() {
	
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void EnemyBulletManager::Draw(Camera& camera, const LightMode& lightType) {
	for (auto& bullet : bullets_) {
		bullet->Draw(camera, lightType);
	}
}

void EnemyBulletManager::ShotBullet(const Vector3& position, const Vector3& direction, const float& speed, const float& size) {
	for (auto& bullet : bullets_) {
		if (!bullet->isActive_) {
			bullet->Shot(position, direction, speed, size);
			break;
		}
	}
}
