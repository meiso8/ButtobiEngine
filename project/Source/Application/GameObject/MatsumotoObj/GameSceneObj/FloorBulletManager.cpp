#include "FloorBulletManager.h"

namespace {
	const int kMaxBullets = 128;
}

FloorBulletManager::FloorBulletManager() {
	for (int i = 0; i < kMaxBullets; i++) {
		bullets_.emplace_back(std::make_unique<FloorBullet>());
		bullets_[i]->Initialize();
	}
}

FloorBulletManager::~FloorBulletManager() {
}

void FloorBulletManager::Initialize() {
	for (auto& bullet : bullets_) {
		bullet->Initialize();
	}
}

void FloorBulletManager::Update() {
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void FloorBulletManager::Draw(Camera& camera, const LightMode& lightType) {
	for (auto& bullet : bullets_) {
		bullet->Draw(camera, lightType);
	}
}

void FloorBulletManager::ShotBullet(const Vector3& position, const Vector3& direction, const float& speed, const float& size) {
	for (auto& bullet : bullets_) {
		if (!bullet->isActive_) {
			bullet->Shot(position, direction, speed, size);
			break;
		}
	}
}
