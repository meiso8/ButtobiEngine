#include "EnemyBombManager.h"


EnemyBombManager::EnemyBombManager() {
    for (int i = 0; i < kMaxBombs; i++) {
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
}

void EnemyBombManager::Draw(Camera& camera) {
    for (auto& bomb : bombs_) {
        bomb->Draw(camera);
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

void EnemyBombManager::ClearBombs()
{
	for (auto& bomb : bombs_) {
		bomb->isActive_ = false;
	}
}
