#pragma once
#include<array>
#include"ParticleEmitter.h"
#include<memory>
#include<vector>
class Camera;
class FloorGamePlayer;
class Enemy;
class EnemyShockWaveManager;
class EmitterManager
{
public:
    EmitterManager(FloorGamePlayer& player, Enemy&enemy, EnemyShockWaveManager& enemyShockWaveManager);
    void Initialize();
    void Update(Camera& camera);
    void Draw();
    void Debug();
private:

    FloorGamePlayer* player_ = nullptr;
    Enemy* enemy_ = nullptr;
    EnemyShockWaveManager* enemyShockWaveManager_;
    enum EmitterType {
        kPlayerWalkEmitter,
        kPlayerHitEmitter,
        kEnemyHitEmitter,
        kEnemyWingEmitter,
        kMaxEmitter
    };
    //std::vector <std::unique_ptr<ParticleEmitter>> bombEmitter_;
    std::array< std::unique_ptr<ParticleEmitter>, kMaxEmitter>particleEmitters_;
    std::vector <std::unique_ptr<ParticleEmitter>> waveEmitter_;
};

