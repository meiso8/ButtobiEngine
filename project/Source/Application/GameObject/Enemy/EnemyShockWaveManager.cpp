#include "EnemyShockWaveManager.h"
#include "MatsumotoObj/GameSceneObj/AttackAreaEmitter.h"

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

void EnemyShockWaveManager::Draw(Camera& camera) {
    for (auto& wave : waves_) {
        wave->Draw(camera);
    }
}

void EnemyShockWaveManager::ShotWave(const Vector3& startPos, const Vector3& endPos, const EnemyShockWave::AABBType& type) {

    for (auto& wave : waves_) {
        if (!wave->isActive_) {
            wave->Shot(startPos, endPos, type);
			AttackAreaEmitter::GetInstance().EmitSquareForm(
                startPos,
				Normalize(Vector2(endPos.z - startPos.z, endPos.x - startPos.x)),
                Vector2(1.5f, Length((endPos - startPos))), 2.1f);
            break;
        }
    }


}
