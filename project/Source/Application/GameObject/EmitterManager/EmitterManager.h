#pragma once
#include<array>
#include"ParticleEmitter.h"
#include<memory>
#include<vector>
class Camera;
class FloorGamePlayer;
class Enemy;
class EnemyShockWaveManager;
class EnemyShockWave;

struct WaveEmitterPair {
    EnemyShockWave* wave;
    std::unique_ptr<ParticleEmitter> emitter;
};

class EmitterManager
{
public:
    EmitterManager(FloorGamePlayer& player, Enemy&enemy, EnemyShockWaveManager& enemyShockWaveManager);
    void Initialize();
    void Update(Camera& camera);
    void Draw();
    void Debug();
private:


    std::vector<WaveEmitterPair> waveEmitters_;

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
};

