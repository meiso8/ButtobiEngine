#include "EnemyShockWaveManager.h"

EnemyShockWaveManager::EnemyShockWaveManager() {
    for (int i = 0; i < kMaxWaves; i++) {
        waves_.emplace_back(std::make_unique<EnemyShockWave>());
        waves_[i]->Initialize();
    }
}

EnemyShockWaveManager::~EnemyShockWaveManager() {
}

void EnemyShockWaveManager::Initialize() {
    for (auto& wave : waves_) {
        wave->Initialize();
    }
}

void EnemyShockWaveManager::Update() {

    for (auto& wave : waves_) {
        wave->Update();
    }

}

void EnemyShockWaveManager::Draw(Camera& camera, const LightMode& lightType) {
    for (auto& wave : waves_) {
        wave->Draw(camera, lightType);
    }
}

void EnemyShockWaveManager::ShotWave(const Vector3& startPos, const Vector3& endPos, const EnemyShockWave::AABBType& type) {

    for (auto& wave : waves_) {
        if (!wave->isActive_) {
            wave->Shot(startPos, endPos, type);
            break;
        }
    }


}
