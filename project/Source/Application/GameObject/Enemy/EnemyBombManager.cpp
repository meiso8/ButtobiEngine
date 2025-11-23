#include "EnemyBombManager.h"

namespace {
    const int kMaxBullets = 20;
}

EnemyBombManager::EnemyBombManager() {
    for (int i = 0; i < kMaxBullets; i++) {
        bombs_.emplace_back(std::make_unique<EnemyBomb>());
        bombs_[i]->Initialize();
    }
}

EnemyBombManager::~EnemyBombManager() {
}

void EnemyBombManager::Initialize() {
    for (auto& bomb : bombs_) {
        bomb->Initialize();
    }
}

void EnemyBombManager::Update() {

    for (auto& bomb : bombs_) {
        bomb->Update();
    }

    isBombReset_ = true;

    for (auto& bomb : bombs_) {
        if (!bomb->isActive_) {
            isBombReset_ = false;
            break;
        }
    }

}

void EnemyBombManager::Draw(Camera& camera, const LightMode& lightType) {
    for (auto& bomb : bombs_) {
        bomb->Draw(camera, lightType);
    }
}

void EnemyBombManager::ShotBomb(const Vector3& startPos, const Vector3& endPos, const float& size) {
    for (auto& bomb : bombs_) {
        if (!bomb->isActive_) {
            bomb->Shot(startPos, endPos, size);
            break;
        }
    }
}
