
#pragma once
#include"EnemyShockWave.h"
#include <vector>

class EnemyShockWaveManager
{
public:
    static const int kMaxWaves =8;
    EnemyShockWaveManager();
    ~EnemyShockWaveManager();
    void Initialize();
    void Update();
    void Draw(class Camera& camera);
    void ShotWave(const Vector3& startPpos, const Vector3& endPos, const EnemyShockWave::AABBType& type);
    const std::vector<std::unique_ptr<EnemyShockWave>>& GetWaves() const {
        return waves_;
    };

private:
    std::vector<std::unique_ptr<EnemyShockWave>> waves_;
};